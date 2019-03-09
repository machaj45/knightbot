# KnightBot
To be able to clone, commit and push changes to repositories on [our git](https://gitlab.fel.cvut.cz/machaj45/knightbot) you ought to set up ssh keys. Generate new ssh key as
```bash
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```

and add contents of a file **~/.ssh/id_rsa.pub** as a new key to your account at https://gitlab.fel.cvut.cz/profile/keys .


```bash
cat ~/.ssh/id_rsa.pub
```

