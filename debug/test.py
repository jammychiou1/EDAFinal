from sage.all import Matrix

def read_mat(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    A_list = []
    for line in lines:
        line = line.strip()
        row = list(map(int, line.split()))
        A_list.append(row)

    return Matrix(A_list)

def go(A):
    for t in range(100):
        A = A.LLL()
        print(A[0])

        if abs(A[0, d]) == weight_y:
            break
        A[0, d + 1 + t] = weight

def test1():
    A = read_mat('a.txt')
    # print(A)

    rows = A.nrows()
    cols = A.ncols()
    d = rows - 1

    weight = 512
    weight_y = A[d, d]
    # print(weight_y)

R = read_mat('r.txt')
null = read_mat('null.txt')
lat = read_mat('lat.txt')
d = lat.ncols()

print(R)
print(R.determinant())
print(null)
print(null.determinant() ** (1 / d))
print(R * null)

print(lat)
print(lat.determinant() ** (1 / d))
print(lat.LLL())
