const contentEl = document.getElementById('content');
const titleEl = document.getElementById('title');
const subtitleEl = document.getElementById('subtitle');

let state = 'menu';
let selection = 0;

function renderMenu() {
  titleEl.textContent = 'Breadstick';
  subtitleEl.textContent = 'Pocket app preview';
  contentEl.innerHTML = `
    <div class="row active"><span class="pill">Menu</span>Choose a tiny app</div>
    <div class="row ${selection === 0 ? 'active' : ''}">1. Tarot</div>
    <div class="row ${selection === 1 ? 'active' : ''}">2. Soon</div>
    <div class="row">Focus: ${selection === 0 ? 'Tarot' : 'Coming soon'}</div>
  `;
}

function renderTarot() {
  titleEl.textContent = 'Tarot';
  subtitleEl.textContent = 'A small, calm draw';
  contentEl.innerHTML = `
    <div class="card-panel">
      <div class="card-title">The Fool</div>
      <div class="card-subtitle">A new beginning</div>
      <div class="row active"><span class="pill">Card</span>Drawn for now</div>
    </div>
    <div class="row">Btn B reveals the reading</div>
  `;
}

function renderReading() {
  titleEl.textContent = 'Tarot';
  subtitleEl.textContent = 'Reading';
  contentEl.innerHTML = `
    <div class="card-panel">
      <div class="card-title">Reading</div>
      <div class="card-subtitle">Upright</div>
      <div class="row active"><span class="pill">Theme</span>Begin again with curiosity and care.</div>
    </div>
    <div class="row">Btn B returns to the start</div>
  `;
}

function renderIdle() {
  titleEl.textContent = 'Tarot';
  subtitleEl.textContent = 'Shake gently';
  contentEl.innerHTML = `
    <div class="row">Start a simple draw</div>
    <div class="row">Btn B uses a placeholder draw</div>
  `;
}

function render() {
  if (state === 'menu') {
    renderMenu();
  } else if (state === 'tarot') {
    renderTarot();
  } else if (state === 'reading') {
    renderReading();
  } else {
    renderIdle();
  }
}

document.getElementById('buttonA').addEventListener('click', () => {
  if (state === 'menu') {
    selection = (selection + 1) % 2;
    render();
  } else {
    state = 'menu';
    render();
  }
});

document.getElementById('buttonB').addEventListener('click', () => {
  if (state === 'menu') {
    state = 'tarot';
    render();
  } else if (state === 'tarot') {
    state = 'reading';
    render();
  } else if (state === 'reading') {
    state = 'menu';
    render();
  }
});

document.getElementById('buttonShake').addEventListener('click', () => {
  if (state === 'menu') {
    state = 'tarot';
  } else {
    state = 'tarot';
  }
  render();
});

render();
