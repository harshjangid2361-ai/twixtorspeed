# GitHub Setup - TwixtorSpeed Plugin

## Step 1: Create GitHub Account

1. Go to https://github.com
2. Click "Sign up"
3. Enter email, password, username
4. Verify email
5. Done! ✅

---

## Step 2: Create New Repository

1. Go to https://github.com/new
2. Repository name: `TwixtorSpeed`
3. Description: `Twixtor Speed - After Effects Plugin`
4. Public (so you can download)
5. Click "Create repository"

---

## Step 3: Upload Files

### Option A: Upload via GitHub Web (EASIEST!)

1. Open your new repository
2. Click "Add file" → "Upload files"
3. Drag-drop this entire folder's contents
4. Commit changes (scroll down, click "Commit changes")

### Option B: Use Git Command Line (HARDER)

```bash
git clone https://github.com/YOUR_USERNAME/TwixtorSpeed.git
cd TwixtorSpeed
# Copy all files from this folder here
git add .
git commit -m "Initial commit"
git push
```

---

## Step 4: GitHub Actions Runs Automatically!

After you upload:

1. Go to your repository
2. Click "Actions" tab
3. You'll see "Build TwixtorSpeed Plugin" running
4. Wait for it to complete (5-10 minutes)
5. Look for green checkmark ✅

---

## Step 5: Download Plugin

When build is complete:

1. Go to "Actions"
2. Click the successful build
3. Scroll down to "Artifacts"
4. Download "TwixtorSpeed.aex"

---

## Step 6: Install in After Effects

1. Copy downloaded .aex file to:
   ```
   C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\
   ```

2. Restart After Effects

3. Go to: Effects > Custom > Twixtor Speed

---

## Important Notes

⚠️ The GitHub Actions build might fail because:
- Adobe SDK is not automatically downloaded
- You need to set it up manually first

**SOLUTION**: Keep using the local build.bat method!

This is just an alternative if you want to try cloud building.

---

## Questions?

GitHub documentation: https://docs.github.com
