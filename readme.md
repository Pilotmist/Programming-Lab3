cd C:\Labs\lab3

Remove-Item -Recurse -Force build

mkdir build

cd build

cmake .. -G "MinGW Makefiles"

cmake --build . --config Release -j 8

ctest --verbose

.\lab3.exe
















































































git status

git add .

git commit -m "Update"

git push origin main




git init


git branch -M main


git remote add origin https://github.com/Precoldesniychel/lab2.git


git add .


git commit -m "Update"


git push -u origin main --force