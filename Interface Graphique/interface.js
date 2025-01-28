/**
 * Script principal pour le détecteur de plagiat
 * Gère l'interface utilisateur, les requêtes API et l'affichage des résultats
 */

// Récupération des éléments du DOM
const elements = {
  titleDoc: document.getElementById("documentTitle"),
  typeDoc: document.getElementById("documentType"),
  table: document.getElementById("scoreTable"),
  finalScore: document.getElementById("finalScore"),
  detectButton: document.getElementById("detectButton"),
  textInput: document.getElementById("textInput"),
  fileInput: document.getElementById("fileInput"),
  typeSelect: document.getElementById("typeselect"),
  highlightedTextDiv: document.getElementById("highlightedText"),
  pdfContainer: document.getElementById("pdf")
};

/**
* Lecteur de fichiers
* @param {File} file - Fichier à lire
* @returns {Promise<string>} Contenu du fichier
*/
function readFile(file) {
  return new Promise((resolve, reject) => {
      const reader = new FileReader();
      reader.onload = () => resolve(reader.result);
      reader.onerror = () => reject(reader.error);
      reader.readAsText(file);
  });
}

/**
* Affiche le texte avec les passages plagiés surlignés
* @param {string} highlightedText - Texte avec balises de surlignage
*/
function displayHighlightedText(highlightedText) {
  const highlightClasses = {
      red: 'highlight-red',
      yellow: 'highlight-yellow',
      magenta: 'highlight-magenta'
  };

  elements.highlightedTextDiv.innerHTML = highlightedText
      .replace(/<red>(.*?)<\/red>/g, `<span class="${highlightClasses.red}">$1</span>`)
      .replace(/<yellow>(.*?)<\/yellow>/g, `<span class="${highlightClasses.yellow}">$1</span>`)
      .replace(/<magenta>(.*?)<\/magenta>/g, `<span class="${highlightClasses.magenta}">$1</span>`);
}

/**
* Envoie une requête au serveur pour détecter le plagiat
* @param {string} content - Contenu à analyser
* @param {string} selectedType - Type de document
* @returns {Promise<Object>} Résultats de l'analyse
*/
async function detectPlagiarism(content, selectedType) {
  const request = {
      method: "POST",
      headers: {
          "Content-Type": "application/json",
      },
      body: JSON.stringify({
          text: content,
          type: selectedType,
      }),
  };

  const response = await fetch("http://localhost:8080/detect_plagiarism", request);
  return response.json();
}

/**
* Affiche les résultats de l'analyse
* @param {Object} result - Résultats de l'analyse
* @param {string} fileName - Nom du fichier analysé
* @param {string} selectedType - Type de document
*/
function displayResults(result, fileName, selectedType) {
  // Affichage des informations de base
  elements.titleDoc.textContent = fileName || "";
  elements.typeDoc.textContent = selectedType || "";
  
  // Affichage des scores dans le tableau
  elements.table.innerHTML = "";
  (result.scores || []).forEach(([name, score]) => {
      const row = elements.table.insertRow();
      row.insertCell().textContent = name;
      row.insertCell().textContent = `${(score * 100).toFixed(2)}%`;
  });
  
  // Affichage du score final
  elements.finalScore.textContent = `${(result.final_score * 100).toFixed(2)}%`;
  
  // Affichage du texte surligné
  displayHighlightedText(result.highlighted_text);
}

// Gestionnaire d'événement principal pour la détection de plagiat
elements.detectButton.addEventListener("click", async () => {
  try {
      // Récupération des entrées utilisateur
      const textInput = elements.textInput.value;
      const fileInput = elements.fileInput.files[0];
      const selectedType = elements.typeSelect.value;

      // Vérification et préparation du contenu
      let content, fileName;
      if (fileInput) {
          content = await readFile(fileInput);
          fileName = fileInput.name.replace(/\.[^/.]+$/, "");
      } else if (textInput) {
          content = textInput;
          fileName = "texte_utilisateur";
      } else {
          alert("Veuillez saisir du texte ou uploader un fichier.");
          return;
      }

      // Détection du plagiat et affichage des résultats
      const result = await detectPlagiarism(content, selectedType);
      displayResults(result, fileName, selectedType);
  } catch (error) {
      console.error("Erreur lors de la détection :", error);
      alert("Une erreur est survenue lors de la détection du plagiat.");
  }
});

// Configuration de l'export PDF
const exportButton = document.createElement("button");
exportButton.id = "exportPdfButton";
exportButton.textContent = "Export PDF";
elements.pdfContainer.appendChild(exportButton);

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