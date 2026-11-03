---
layout: default
title: GitHub Pages Setup
---
# Publish this separate tutorial with GitHub Pages

1. Create a new GitHub repository called **`TCS34725-Tutorial`**.
2. Upload this archive's contents to that repo (not the main project repo). The repository root must contain `README.md`, `docs/`, and `examples/`.
3. Commit to the `main` branch.
4. Open **Settings → Pages**.
5. Under **Build and deployment**, select **Deploy from a branch**; choose `main` and `/docs`; Save.
6. Open `https://YOUR-USERNAME.github.io/TCS34725-Tutorial/` after deployment completes.

The website is **publicly accessible** when published. It contains documentation, not a live C runtime. Example source files are available in the GitHub repo.

The `docs/` folder uses Markdown pages with Jekyll front matter and a simple GitHub Pages-compatible theme. The tutorials use `.html` links for the generated Pages URLs. When browsing Markdown directly on GitHub, use README's `.md` links instead.

For an alternate repository name or base path, GitHub Pages uses that repository name in its usual project-site URL.
