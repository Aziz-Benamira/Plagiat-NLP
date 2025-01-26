// script.js
let titleDoc = document.getElementById("documentTitle");
let typeDoc = document.getElementById("documentType");
let table = document.getElementById("scoreTable");
let finalScore = document.getElementById("finalScore");
document.getElementById("detectButton").addEventListener("click", async () => {
  const textInput = document.getElementById("textInput").value;
  const fileInput = document.getElementById("fileInput").files[0];
  const selectedType = document.getElementById("typeselect").value;
  let content;
  let fileName;
  if (fileInput) {
    // Si un fichier est uploadé, lire son contenu
    content = await readFile(fileInput);
    fileName = fileInput.name.replace(/\.[^/.]+$/, "");
  } else if (textInput) {
    // Si du texte est saisi, utiliser ce texte
    content = textInput;
    fileName = "texte_utilisateur";
  } else {
    alert("Veuillez saisir du texte ou uploader un fichier.");
    return;
  }

  // Envoyer le contenu au backend pour détection de plagiat
  let request = {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      text: content,
      type: selectedType,
    }),
  };
  console.log(request);
  const response = await fetch(
    "http://localhost:8080/detect_plagiarism",
    request
  );

  const result = await response.json();
  console.log(result);
  titleDoc.textContent = fileName || "";
  typeDoc.textContent = selectedType || "";
  table.innerHTML = "";
  (result.scores || []).forEach(([name, score]) => {
    const row = table.insertRow();
    row.insertCell().textContent = name;
    row.insertCell().textContent = `${(score * 100).toFixed(2)}%`;
  });
  finalScore.textContent = `${(result.final_score * 100).toFixed(2)}%`;

  // Afficher le texte surligné
  displayHighlightedText(result.highlighted_text);
});

// Fonction pour lire un fichier texte
function readFile(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => resolve(reader.result);
    reader.onerror = () => reject(reader.error);
    reader.readAsText(file);
  });
}

// Fonction pour afficher le texte surligné
function displayHighlightedText(highlightedText) {
  const highlightedTextDiv = document.getElementById("highlightedText");
  highlightedTextDiv.innerHTML = highlightedText
    .replace(/<red>(.*?)<\/red>/g, '<span class="highlight-red">$1</span>')
    .replace(
      /<yellow>(.*?)<\/yellow>/g,
      '<span class="highlight-yellow">$1</span>'
    )
    .replace(
      /<magenta>(.*?)<\/magenta>/g,
      '<span class="highlight-magenta">$1</span>'
    );
}

const exportButton = document.createElement("button");
exportButton.id = "exportPdfButton";
exportButton.textContent = "Export Pdf";

document.getElementById("pdf").appendChild(exportButton);

exportButton.addEventListener("click", () => {
  const { jsPDF } = window.jspdf;
  const pdf = new jsPDF("p", "pt", "a4");
  const content = document.getElementById("result");
  pdf.html(content, {
    callback: (doc) => {
      doc.save("rapport.pdf");
    },
    x: 10,
    y: 10,
    html2canvas: { scale: 0.55 },
  });
});
