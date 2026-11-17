
import { useState, type ChangeEvent } from "react";
import "./App.css";

type InputMode = "upload" | "paste" | "random";
type MainTab = "create" | "solve";
type ResultView = "puzzle" | "answers";

type Puzzle = {
  width: number;
  height: number;
  words: string[];
  letters: string[];
  solution: number[][][];
};

function App() {
  const [mainTab, setMainTab] = useState<MainTab>("create");  
  const [mode, setMode] = useState<InputMode>("upload");
  const [csvText, setCsvText] = useState("");
  const [width, setWidth] = useState(15);
  const [height, setHeight] = useState(15);
  const [wordCount, setWordCount] = useState(15);
  const [puzzle, setPuzzle] = useState<Puzzle | null>(null);
  const [error, setError] = useState("");
  const [fileName, setFileName] = useState("");
  const [loading, setLoading] = useState(false);
  const [createView, setCreateView] = useState<ResultView>("puzzle");


  const [solveCsvText, setSolveCsvText] = useState("");
  const [solveFileName, setSolveFileName] = useState("");
  const [solvedPuzzle, setSolvedPuzzle] = useState<Puzzle | null>(null);
  const [solveError, setSolveError] = useState("");
  const [solveLoading, setSolveLoading] = useState(false);
  const [solveView, setSolveView] = useState<ResultView>("puzzle");

  async function submitPuzzle(contents?: string) {
    setLoading(true);
    setError("");
    setPuzzle(null);

    try {
      const response = await fetch("/api/puzzles", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(
          mode === "random"
            ? { mode: "random", width, height, wordCount }
            : { mode: "csv", csvText: contents ?? csvText }
        ),
      });

      const result = await response.json() as Puzzle | { error: string };

      if (!response.ok || "error" in result) {
        throw new Error("error" in result ? result.error : "Unable to create puzzle.");
      }

      setPuzzle(result);
      setCreateView("puzzle");
    } catch (requestError) {
      setError(requestError instanceof Error ? requestError.message : "Unable to contact the backend.");
    } finally {
      setLoading(false);
    }
  }

  async function handleFileUpload(event: ChangeEvent<HTMLInputElement>) {
    const file = event.target.files?.[0];
    if (!file) return;

    setFileName(file.name);
    const contents = await file.text();
    setCsvText(contents);
    await submitPuzzle(contents);
  }


  async function submitSolve(contents?: string) {
    setSolveLoading(true);
    setSolveError("");
    setSolvedPuzzle(null);

    try {
      const response = await fetch("/api/puzzles", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          mode: "csv",
          csvText: contents ?? solveCsvText,
        }),
      });

      const result = await response.json() as Puzzle | { error: string };

      if (!response.ok || "error" in result) {
        throw new Error("error" in result ? result.error : "Unable to solve puzzle.");
      }

      setSolvedPuzzle(result);
      setSolveView("answers");
    } catch (requestError) {
      setSolveError(requestError instanceof Error ? requestError.message : "Unable to contact the backend.");
    } finally {
      setSolveLoading(false);
    }
  }

  async function handleSolveFileUpload(event: ChangeEvent<HTMLInputElement>) {
    const file = event.target.files?.[0];
    if (!file) return;

    setSolveFileName(file.name);
    const contents = await file.text();
    setSolveCsvText(contents);
    await submitSolve(contents);
  }

  function changeMode(newMode: InputMode) {
    setMode(newMode);
    setPuzzle(null);
    setError("");
  }

  function renderPuzzle(currentPuzzle: Puzzle, view: ResultView) {
    const highlightedCells = new Map<string, number>();

    currentPuzzle.solution.forEach((positions, wordIndex) => {
      positions.forEach(([row, col]) => {
        highlightedCells.set(`${row}-${col}`, wordIndex);
      });
    });

    return (
      <>
        <div className="result-tabs"> 
          <button
            className={view === "puzzle" ? "result-tab active" : "result-tab"}
            onClick={() => mainTab === "create" ? setCreateView("puzzle") : setSolveView("puzzle")}
          >
            Puzzle
          </button>
          <button
            className={view === "answers" ? "result-tab active" : "result-tab"}
            onClick={() => mainTab === "create" ? setCreateView("answers") : setSolveView("answers")}
          >
            Answers
          </button>
        </div>

        {view === "puzzle" && (
          <>
            <h4>Word Search</h4>
            <div className="puzzle-grid-wrapper">
              <div
                className="puzzle-grid"
                style={{ gridTemplateColumns: `repeat(${currentPuzzle.width}, 32px)` }}
              >
                {currentPuzzle.letters.flatMap((row, rowIndex) =>
                  row.split("").map((letter, colIndex) => (
                    <div className="letter-cell" key={`${rowIndex}-${colIndex}`}>
                      {letter}
                    </div>
                  ))
                )}
              </div>
            </div>
          </>
        )}

        {view === "answers" && (
          <>
            <h4>Solution</h4>
            <p className="answer-description">
              The highlighted letters show where each word was found.
            </p>

            <div className="puzzle-grid-wrapper">
              <div
                className="puzzle-grid"
                style={{ gridTemplateColumns: `repeat(${currentPuzzle.width}, 32px)` }}
              >
                {currentPuzzle.letters.flatMap((row, rowIndex) =>
                  row.split("").map((letter, colIndex) => {
                    const wordIndex = highlightedCells.get(`${rowIndex}-${colIndex}`);

                    return (
                      <div
                        className={`letter-cell ${wordIndex !== undefined ? `answer-color-${wordIndex % 6}` : ""}`}
                        key={`${rowIndex}-${colIndex}`}
                        title={wordIndex !== undefined ? currentPuzzle.words[wordIndex] : ""}
                      >
                        {letter}
                      </div>
                    );
                  })
                )}
              </div>
            </div>

            <h4>Found Words</h4>
            <div className="word-list answer-word-list">
              {currentPuzzle.words.map((word, index) => {
                const found = currentPuzzle.solution[index]?.length > 0;

                return (
                  <span
                    className={`answer-color-${index % 6} ${found ? "" : "not-found"}`}
                    key={`${word}-${index}`}
                  >
                    {word} {found ? "✓" : "(Not found)"}
                  </span>
                );
              })}
            </div>
          </>
        )}
      </>
    );
  }

  return (
    <main className="app">
      <header className="header">
        <div>
          <h1>WordSearch</h1>
          <p>Word search puzzle generator and solver</p>
        </div>
      </header>

      
      <nav className="main-tabs">
        <button
          className={mainTab === "create" ? "main-tab active" : "main-tab"}
          onClick={() => setMainTab("create")}
        >
          Create WS
        </button>
        <button
          className={mainTab === "solve" ? "main-tab active" : "main-tab"}
          onClick={() => setMainTab("solve")}
        >
          Solve WS
        </button>
      </nav>

      {mainTab === "create" && (
        <section className="container">
          <div className="section-heading">
            <h2>Create a puzzle</h2>
            <p>Choose how you want to create your word search.</p>
          </div>

          <div className="mode-selector">
            <button className={mode === "upload" ? "mode active" : "mode"} onClick={() => changeMode("upload")}>Upload CSV</button>
            <button className={mode === "paste" ? "mode active" : "mode"} onClick={() => changeMode("paste")}>Paste CSV</button>
            <button className={mode === "random" ? "mode active" : "mode"} onClick={() => changeMode("random")}>Random</button>
          </div>

          <div className="input-panel">
            {mode === "upload" && (
              <div className="upload-section">
                <h3>Upload a CSV file</h3>
                <p>Select an existing word search CSV file.</p>
                <label className="upload-area">
                  <strong>Choose a CSV file</strong>
                  <span>or drag and drop your file here</span>
                  <input type="file" accept=".csv,text/csv" onChange={handleFileUpload} />
                </label>
                {fileName && <p className="file-name">Selected: {fileName}</p>}
              </div>
            )}

            {mode === "paste" && (
              <div>
                <h3>Paste CSV contents</h3>
                <p>Paste your existing word search CSV data below.</p>
                <textarea
                  className="csv-input"
                  value={csvText}
                  onChange={event => setCsvText(event.target.value)}
                  placeholder={"20,12\nword,search,code,class\n..."}
                />
              </div>
            )}

            {mode === "random" && (
              <div>
                <h3>Random puzzle settings</h3>
                <p>Choose the size and number of words.</p>
                <div className="settings-grid">
                  <label>Grid width<input type="number" min="5" max="50" value={width} onChange={event => setWidth(Number(event.target.value))} /></label>
                  <label>Grid height<input type="number" min="5" max="50" value={height} onChange={event => setHeight(Number(event.target.value))} /></label>
                  <label>Number of words<input type="number" min="1" max="100" value={wordCount} onChange={event => setWordCount(Number(event.target.value))} /></label>
                </div>
              </div>
            )}

            {error && <div className="error">{error}</div>}

            {mode !== "upload" && (
              <button className="primary-button" onClick={() => void submitPuzzle()} disabled={loading}>
                {loading ? "Working..." : mode === "random" ? "Generate random puzzle" : "Load puzzle"}
              </button>
            )}

            {mode === "upload" && loading && <p>Loading puzzle...</p>}
          </div>

          {puzzle && (
            <div className="preview">
              <div className="preview-heading">
                <div>
                  <h3>Puzzle loaded successfully</h3>
                  <p>Your puzzle is ready.</p>
                </div>
                <span className="success">Ready</span>
              </div>

              <div className="preview-stats">
                <div><strong>{puzzle.width} x {puzzle.height}</strong><span>Grid size</span></div>
                <div><strong>{puzzle.words.length}</strong><span>Words</span></div>
              </div>

              <h4>Word list</h4>
              <div className="word-list">
                {puzzle.words.map((word, index) => <span key={`${word}-${index}`}>{word}</span>)}
              </div>

              {renderPuzzle(puzzle, createView)}

              <button className="secondary-button" onClick={() => setPuzzle(null)}>Clear Puzzle</button>
            </div>
          )}
        </section>
      )}

      
      {mainTab === "solve" && (
        <section className="container">
          <div className="section-heading">
            <h2>Solve a word search</h2>
            <p>Upload or paste an existing puzzle to find the hidden words.</p>
          </div>

          <div className="input-panel">
            <div className="upload-section">
              <h3>Upload a CSV file</h3>
              <p>Select a CSV containing the puzzle dimensions, word list, and letters.</p>
              <label className="upload-area">
                <strong>Choose a CSV file</strong>
                <span>or drag and drop your file here</span>
                <input type="file" accept=".csv,text/csv" onChange={handleSolveFileUpload} />
              </label>
              {solveFileName && <p className="file-name">Selected: {solveFileName}</p>}
            </div>

            <div className="solve-paste-section">
              <h3>Or paste CSV contents</h3>
              <textarea
                className="csv-input"
                value={solveCsvText}
                onChange={event => setSolveCsvText(event.target.value)}
                placeholder={"20,12\nword,search,code,class\n..."}
              />
              <button
                className="primary-button"
                onClick={() => void submitSolve()}
                disabled={solveLoading || !solveCsvText.trim()}
              >
                {solveLoading ? "Solving..." : "Solve Word Search"}
              </button>
            </div>

            {solveError && <div className="error">{solveError}</div>}
          </div>

          {solvedPuzzle && (
            <div className="preview">
              <div className="preview-heading">
                <div>
                  <h3>Word search solved</h3>
                  <p>The backend searched for the words in your puzzle.</p>
                </div>
                <span className="success">Solved</span>
              </div>

              <div className="preview-stats">
                <div><strong>{solvedPuzzle.width} x {solvedPuzzle.height}</strong><span>Grid size</span></div>
                <div><strong>{solvedPuzzle.words.length}</strong><span>Words in list</span></div>
                <div>
                  <strong>{solvedPuzzle.solution.filter(positions => positions.length > 0).length}</strong>
                  <span>Words found</span>
                </div>
              </div>

              {renderPuzzle(solvedPuzzle, solveView)}

              <button className="secondary-button" onClick={() => setSolvedPuzzle(null)}>Clear Puzzle</button>
            </div>
          )}
        </section>
      )}
    </main>
  );
}

export default App;
