
import { createServer } from "node:http";
import { spawn } from "node:child_process";
import { existsSync } from "node:fs";
import { fileURLToPath } from "node:url";
import path from "node:path";

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");
const executable = path.join(root, process.platform === "win32" ? "wordsearch-api.exe" : "wordsearch-api");
const port = Number(process.env.API_PORT || 3001);

function runEngine(input) {
  return new Promise((resolve, reject) => {
    if (!existsSync(executable)) {
      reject(new Error("Backend executable not found. Compile api.cpp first; see README.md."));
      return;
    }
    const process = spawn(executable, [], { cwd: root, windowsHide: true });
    let output = "";
    let errorOutput = "";
    process.stdout.setEncoding("utf8");
    process.stderr.setEncoding("utf8");
    process.stdout.on("data", chunk => output += chunk);
    process.stderr.on("data", chunk => errorOutput += chunk);
    process.on("error", reject);
    process.on("close", code => {
      if (code !== 0 && !output) reject(new Error(errorOutput || `Engine exited with code ${code}.`));
      else {
        try { resolve(JSON.parse(output)); }
        catch { reject(new Error(errorOutput || "The C++ backend returned an invalid response.")); }
      }
    });
    process.stdin.end(input);
  });
}

const server = createServer(async (request, response) => {
  response.setHeader("Content-Type", "application/json; charset=utf-8");
  response.setHeader("Access-Control-Allow-Origin", "http://localhost:5173");
  response.setHeader("Access-Control-Allow-Headers", "Content-Type");
  if (request.method === "OPTIONS") { response.writeHead(204).end(); return; }
  if (request.method !== "POST" || request.url !== "/api/puzzles") {
    response.writeHead(404).end(JSON.stringify({ error: "Route not found." }));
    return;
  }
  try {
    let body = "";
    for await (const chunk of request) body += chunk;
    const payload = JSON.parse(body);
    let input;
    if (payload.mode === "csv" && typeof payload.csvText === "string") input = `csv\n${payload.csvText}`;
    else if (payload.mode === "random") input = `random\n${payload.width}\n${payload.height}\n${payload.wordCount}\n`;
    else throw new Error("Invalid puzzle request.");
    const result = await runEngine(input);
    response.writeHead(result.error ? 400 : 200).end(JSON.stringify(result));
  } catch (error) {
    response.writeHead(400).end(JSON.stringify({ error: error instanceof Error ? error.message : "Request failed." }));
  }
});

server.listen(port, () => console.log(`WordSearch API listening on http://localhost:${port}`));
