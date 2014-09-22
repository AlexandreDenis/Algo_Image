git init
git config --global user.name "calime"
git config --global user.email "adrien.calime@gmail.com"
git config --global credential.helper cache
git config --global credential.helper 'cache --timeout=3600'
git remote add origin $1
git pull origin master
