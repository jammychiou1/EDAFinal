from subprocess import run
for i in range(1, 21):
    case = str(i).zfill(2)
    run(f'echo "test{case}" && ./build/Release/apps/main -input circuits/test{case}/top_primitive.v -output debug/test.v && cat debug/test.v && echo ""', shell=True)
