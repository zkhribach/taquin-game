Sure! Here's a well-structured **README.md** file to guide your team on how to use Git for the project.

---

### **README.md**

```markdown
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

---

### 2. **Create Your Own Branch**
Never work directly on the `main` branch. Create your own branch for each task you are working on.

Example:
```bash
git checkout -b your-branch-name
```
Replace `your-branch-name` with something meaningful:
- `game-logic`
- `input-output`
- `file-save-load`
- `testing`

---

### 3. **Make Changes**
1. Write or modify the code in your branch.
2. Stage the changes (prepare them for commit):
   ```bash
   git add .
   ```

3. Commit your changes with a meaningful message:
   ```bash
   git commit -m "Short description of the changes"
   ```

Example commit message:
```bash
git commit -m "Add function to shuffle the board"
```

---

### 4. **Push Your Branch to GitHub**
Upload your branch to GitHub so others can see your changes:
```bash
git push origin your-branch-name
```

---

### 5. **Create a Pull Request (PR)**
Once your changes are ready:
1. Go to [GitHub Repository](https://github.com/zkhribach/taquin-game).
2. You will see a **"Compare & Pull Request"** button. Click it.
3. Write a description of what you did and submit the PR.

A team member will review your PR, suggest changes if needed, and merge it into the `main` branch.

---

## 🔄 Stay Updated with Main Branch

Before starting new work or to avoid conflicts, always update your local `main` branch:

1. Switch to the `main` branch:
   ```bash
   git checkout main
   ```

2. Pull the latest changes:
   ```bash
   git pull origin main
   ```

3. Update your working branch:
   ```bash
   git checkout your-branch-name
   git merge main
   ```

---

## 🛠️ Basic Git Commands Cheat Sheet

| Command                             | Description                              |
|-------------------------------------|------------------------------------------|
| `git status`                        | Check the status of your changes         |
| `git add .`                         | Stage all changes                        |
| `git commit -m "message"`           | Commit changes with a message            |
| `git checkout -b branch-name`       | Create and switch to a new branch        |
| `git push origin branch-name`       | Push your branch to GitHub               |
| `git pull origin main`              | Pull the latest changes from `main`      |
| `git merge main`                    | Merge `main` into your current branch    |

---

## 🤝 Collaboration Guidelines

1. Always create a new branch for your work.
2. Commit often with clear, descriptive messages.
3. Pull updates from `main` before pushing your changes.
4. Review pull requests carefully before merging.

---

## 📧 Need Help?

If you get stuck or need help:
- Reach out to the team on our communication channel.
- Use `git status` and `git log` to troubleshoot issues.

Happy coding! 🚀
