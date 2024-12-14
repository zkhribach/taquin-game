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
