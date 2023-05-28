import subprocess

def run_get_private_max(path: str, param: str):
    try:
        process = subprocess.Popen([path, param], stdout=subprocess.PIPE, text=True)
        process.wait()

        if process.returncode == 0:
            output = process.stdout.read().strip()
            value = int(output)
            return value
        else:
            print("Error: Program execution failed.")
    except FileNotFoundError:
        print("Error: Program not found.")

def mem_test_algo(algo: str):
    path = algo + "\\bench.exe"
    avg_def = 0
    avg_algo = 0
    times_run = 0

    while avg_algo <= avg_def:
        times_run += 800
        
        for i in range(800):
            memm = run_get_private_max(path, "round")
            memd = run_get_private_max(path, "runempty")
            avg_algo += memm
            avg_def += memd

    avg_algo /= times_run
    avg_def /= times_run
        
    print(algo, "memory usage")
    print("Private memory without algo:", avg_def)
    print("Private memory with algo:", avg_algo)
    print("Private memory algo:", avg_algo - avg_def)
    

mem_test_algo("ecdsa")
mem_test_algo("fa3x")
mem_test_algo("ffs")
mem_test_algo("gps")
mem_test_algo("gq")
mem_test_algo("schnorr")