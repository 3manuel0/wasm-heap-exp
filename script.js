const terminal = document.getElementById("terminal");

const worker = new Worker("worker.js");

worker.postMessage({
  type: "init",
});

worker.postMessage({
  type: "run_test",
});

worker.onmessage = (e) => {
  const { type, error, result, term } = e.data;

  switch (type) {
    case "stdout":
      // writing to the fake terminal
      terminal.textContent += term;
      // scrolling
      terminal.scrollTop = terminal.scrollHeight;

      break;

    case "ready":
      console.log("WASM is loaded and heap_base is:", result);
      break;

    case "error":
      console.error("Worker Error:", error);
      break;
  }
};
