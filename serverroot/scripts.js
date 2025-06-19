document.addEventListener("DOMContentLoaded", function () {
  document.getElementById('toggleComic').addEventListener('click', () => {
    const comicCard = document.getElementById('comicCard');
    comicCard.classList.toggle('hidden');
  });
})