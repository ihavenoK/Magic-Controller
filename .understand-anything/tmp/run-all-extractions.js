const fs = require('fs');
const { execSync } = require('child_process');
const path = require('path');

const projectRoot = 'D:/32/cyberrypotter';
const skillDir = 'C:/Users/ihavenoK/.gemini/antigravity/skills/understand-anything/understand';
const intermediateDir = path.join(projectRoot, '.understand-anything/intermediate');
const tmpDir = path.join(projectRoot, '.understand-anything/tmp');

if (!fs.existsSync(tmpDir)) {
  fs.mkdirSync(tmpDir, { recursive: true });
}

// Load batches
const batchesData = JSON.parse(fs.readFileSync(path.join(intermediateDir, 'batches.json'), 'utf8'));
const batches = batchesData.batches;

console.log(`Starting structural extraction for ${batches.length} batches...`);

for (let i = 0; i < batches.length; i++) {
  const batch = batches[i];
  
  // Format input JSON
  const inputData = {
    projectRoot: projectRoot,
    batchFiles: batch.files,
    batchImportData: batch.batchImportData
  };
  
  const inputPath = path.join(tmpDir, `ua-file-analyzer-input-${i}.json`);
  const outputPath = path.join(tmpDir, `ua-file-extract-results-${i}.json`);
  
  fs.writeFileSync(inputPath, JSON.stringify(inputData, null, 2));
  
  const cmd = `node "${path.join(skillDir, 'extract-structure.mjs')}" "${inputPath}" "${outputPath}"`;
  console.log(`Running batch ${i}/${batches.length - 1}...`);
  try {
    execSync(cmd, { stdio: 'inherit' });
  } catch (err) {
    console.error(`Error in batch ${i}:`, err.message);
  }
}

console.log("All structural extractions completed.");
