git init
git config --global user.name "Alexandre Denis"
git config --global user.email "alexandre.denis@dbmail.com"
git config --global credential.helper cache
git config --global credential.helper 'cache --timeout=3600'
git remote add origin $1
git pull origin master
