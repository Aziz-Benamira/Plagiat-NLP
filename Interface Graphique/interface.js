// script.js

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
  console.log(content);
  console.log(selectedType);
  console.log(fileName);
  //   Envoyer le contenu au backend pour détection de plagiat
  //   const response = await fetch("http://localhost:8080/detect_plagiarism", {
  //     method: "POST",
  //     headers: {
  //       "Content-Type": "application/json",
  //     },
  //     body: JSON.stringify({
  //       text: content,
  //       type: selectedType,
  //       title: fileName,
  //     }),
  //   });

  //   const result = await response.json();

  //   // Afficher le texte surligné
  //   displayHighlightedText(result.highlightedText);
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
