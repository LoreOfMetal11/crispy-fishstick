docker volume create test-volume


docker run -dit --name my-container -v test-volume:/data 1teo-mingw sh



docker exec -it my-container sh -c "echo 'Тестинг' > /data/volume-test.txt"



docker rm -f my-container



docker volume lsdocker run -dit --name my-container-new -v test-volume:/data 1teo-mingw sh

