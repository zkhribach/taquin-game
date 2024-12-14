# Taquin Game Project

Welcome to the Taquin Game project! 🎮 This guide will help you get started with Git and GitHub so that our collaboration runs smoothly.

---

## 🚀 Getting Started

### 1. **Clone the Repository**
To start working on the project, you need to copy (clone) the repository to your local machine.

Run this command in your terminal:
```bash
git clone git@github.com:zkhribach/taquin-game.git
cd taquin-game
```
### 2. ***Create Your Own Branch***
```bash
git checkout -b your-branch-name
```
### 3. ***Make Changes***
Stage the changes (prepare them for commit):
```bash
git add .
```
Commit your changes with a meaningful message:
```bash
git commit -m "Short description of the changes"
```
### 4. ***Push Your Branch to GitHub***
Upload your branch to GitHub so others can see your changes:
```bash
git push origin your-branch-name
```

## 🔄 Stay Updated with Main Branch
Before starting new work or to avoid conflicts, always update your local main branch:
Switch to the ```main``` branch:
```bash
git checkout main
```
Pull the latest changes:
```bash

git pull origin main
```

Update your working branch:
```bash
git checkout your-branch-name
git merge main
```
