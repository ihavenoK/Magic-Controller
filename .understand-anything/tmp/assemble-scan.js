const fs = require('fs');

const scanFiles = JSON.parse(fs.readFileSync('D:/32/cyberrypotter/.understand-anything/tmp/ua-scan-files.json', 'utf8'));
const importMapData = JSON.parse(fs.readFileSync('D:/32/cyberrypotter/.understand-anything/tmp/ua-import-map-output.json', 'utf8'));

const scanResult = {
  name: "CyberryPotter",
  description: "Cyberry Potter 电子魔杖基础项目。基于 STM32F103 和 NNoM 神经网络库部署卷积神经网络模型，实现 MPU6050 动作识别与法术分类。",
  languages: ["c", "python", "markdown", "toml", "yaml"],
  frameworks: ["Keil", "STM32", "NNoM", "MPU6050"],
  files: scanFiles.files,
  totalFiles: scanFiles.totalFiles,
  filteredByIgnore: scanFiles.filteredByIgnore,
  estimatedComplexity: scanFiles.estimatedComplexity,
  importMap: importMapData.importMap
};

fs.writeFileSync('D:/32/cyberrypotter/.understand-anything/intermediate/scan-result.json', JSON.stringify(scanResult, null, 2));
console.log("Successfully assembled scan-result.json");
