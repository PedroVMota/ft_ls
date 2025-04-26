#!/usr/bin/env python3
"""
Comprehensive benchmark script for comparing:
1. ft_ls (release) vs ft_ls_debug - performance comparison
2. ft_ls vs system ls - both performance and output comparison

This script verifies both correctness and performance of your ft_ls implementation.
"""

import os
import sys
import time
import subprocess
import difflib
from statistics import mean
import platform
import re

# ANSI color codes
class Colors:
    GREEN = '\033[0;32m'
    CYAN = '\033[0;36m'
    YELLOW = '\033[1;33m'
    RED = '\033[0;31m'
    BLUE = '\033[0;34m'
    MAGENTA = '\033[0;35m'
    NC = '\033[0m'  # No Color

# Check if executables exist
def check_executables():
    if not os.path.isfile("./ft_ls"):
        print(f"{Colors.RED}Error: ft_ls (release version) not found!{Colors.NC}")
        print("Please run 'make release' first.")
        sys.exit(1)
    
    if not os.path.isfile("./ft_ls_debug"):
        print(f"{Colors.RED}Error: ft_ls_debug (debug version) not found!{Colors.NC}")
        print("Please run 'make debug' first.")
        sys.exit(1)

# Test directories to benchmark
TEST_CASES = [
    ".",
    "..",
    # "/usr",
    # "/usr/bin",
    # "/usr/local"
]

# Flags to test
FLAG_COMBINATIONS = [
    "",
    "-l",
    # "-R",
    # "-a",
    # "-la",
    # "-laR"
]

# Function to normalize output
def normalize_output(output):
    """
    Normalize output to make comparison more meaningful:
    - Remove timestamps and date fields (in -l output)
    - Remove user and group names (which might differ)
    - Remove total line which might have different values
    - Remove whitespace variations
    """
    # Convert to string if bytes
    if isinstance(output, bytes):
        output = output.decode('utf-8', errors='replace')
    
    lines = output.split('\n')
    normalized_lines = []
    
    for line in lines:
        # Skip 'total' lines
        if line.startswith('total '):
            continue
            
        # For -l format, normalize the variable parts
        if re.match(r'^[-dl][-rwx]{9}\s+\d+\s+\w+\s+\w+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+.+$', line):
            # Keep permissions, size, and filename but normalize the rest
            parts = line.split()
            if len(parts) >= 9:
                # Keep permissions (parts[0]), file size, and filename
                permissions = parts[0]
                size = parts[4]
                filename = ' '.join(parts[8:])
                normalized_line = f"{permissions} SIZE:{size} NAME:{filename}"
                normalized_lines.append(normalized_line)
            else:
                normalized_lines.append(line)
        else:
            # For regular output, just remove extra whitespace
            normalized_line = re.sub(r'\s+', ' ', line).strip()
            if normalized_line:
                normalized_lines.append(normalized_line)
    
    # Sort the lines to handle order differences
    normalized_lines.sort()
    return '\n'.join(normalized_lines)

# Function to measure execution time and capture output
def run_and_measure(command, args):
    cmd = [command] + args
    
    start_time = time.time()
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=False)
    end_time = time.time()
    
    execution_time = end_time - start_time
    return execution_time, result.stdout

# Function to benchmark two commands and compare output
def compare_commands(cmd1, cmd2, cmd1_name, cmd2_name, test_path, flags):
    flag_list = flags.split() if flags else []
    args = flag_list + [test_path]
    
    print(f"{Colors.CYAN}Testing: {cmd1_name} vs {cmd2_name} with args: {' '.join(args)}{Colors.NC}")
    
    # Run multiple times for timing
    runs = 3
    cmd1_times = []
    cmd2_times = []
    
    # First run captures output
    cmd1_time, cmd1_output = run_and_measure(cmd1, args)
    cmd2_time, cmd2_output = run_and_measure(cmd2, args)
    
    cmd1_times.append(cmd1_time)
    cmd2_times.append(cmd2_time)
    
    # Additional runs for timing
    for i in range(1, runs):
        time1, _ = run_and_measure(cmd1, args)
        time2, _ = run_and_measure(cmd2, args)
        cmd1_times.append(time1)
        cmd2_times.append(time2)
    
    # Calculate average times
    avg_time1 = mean(cmd1_times)
    avg_time2 = mean(cmd2_times)
    
    print(f"  {cmd1_name} avg time: {avg_time1:.6f}s")
    print(f"  {cmd2_name} avg time: {avg_time2:.6f}s")
    
    # Compare performance
    if avg_time2 > 0.001:  # Avoid division by very small numbers
        speed_diff = ((avg_time2 - avg_time1) / avg_time2) * 100
        if speed_diff > 0:
            print(f"{Colors.GREEN}  {cmd1_name} is {abs(speed_diff):.2f}% faster than {cmd2_name}{Colors.NC}")
        else:
            print(f"{Colors.RED}  {cmd1_name} is {abs(speed_diff):.2f}% slower than {cmd2_name}{Colors.NC}")
    
    # Compare output only if we're comparing ft_ls vs system ls
    if cmd1_name == "ft_ls" and cmd2_name == "ls":
        normalized_output1 = normalize_output(cmd1_output)
        normalized_output2 = normalize_output(cmd2_output)
        
        if normalized_output1 == normalized_output2:
            print(f"{Colors.GREEN}  Output matches! ✓{Colors.NC}")
            output_match = True
        else:
            print(f"{Colors.RED}  Output differs! ✗{Colors.NC}")
            
            # Show difference for debugging if output doesn't match
            diff = list(difflib.unified_diff(
                normalized_output2.split('\n'),
                normalized_output1.split('\n'),
                lineterm='',
                fromfile='ls output',
                tofile='ft_ls output'
            ))
            
            # Only show up to 10 lines of diff to avoid spamming the console
            if len(diff) > 10:
                print("  First 10 differences:")
                for line in diff[:10]:
                    if line.startswith('+'):
                        print(f"{Colors.GREEN}  {line}{Colors.NC}")
                    elif line.startswith('-'):
                        print(f"{Colors.RED}  {line}{Colors.NC}")
                    else:
                        print(f"  {line}")
                print(f"  ... and {len(diff) - 10} more differences")
            else:
                print("  Differences:")
                for line in diff:
                    if line.startswith('+'):
                        print(f"{Colors.GREEN}  {line}{Colors.NC}")
                    elif line.startswith('-'):
                        print(f"{Colors.RED}  {line}{Colors.NC}")
                    else:
                        print(f"  {line}")
            
            output_match = False
        
        return speed_diff, output_match
    
    return (avg_time2 - avg_time1) / avg_time2 * 100 if avg_time2 > 0.001 else 0, None

def main():
    # Print header
    print(f"{Colors.YELLOW}=========================================={Colors.NC}")
    print(f"{Colors.YELLOW}  Comprehensive ft_ls Benchmark & Testing  {Colors.NC}")
    print(f"{Colors.YELLOW}=========================================={Colors.NC}")
    print(f"Running on: {platform.system()} {platform.release()}")
    print()
    
    # Check if executables exist
    check_executables()
    
    # Store results for summary
    release_vs_debug_results = []
    ft_ls_vs_ls_perf_results = []
    output_match_count = 0
    total_output_tests = 0
    
    # print(f"{Colors.MAGENTA}Part 1: ft_ls (release) vs ft_ls_debug performance{Colors.NC}")
    # print()
    
    # # First compare release vs debug
    # for test_path in TEST_CASES:
    #     for flags in FLAG_COMBINATIONS:
    #         print(f"{Colors.YELLOW}=== Testing: {flags} {test_path} ==={Colors.NC}")
    #         speed_diff, _ = compare_commands("./ft_ls", "./ft_ls_debug", "ft_ls", "ft_ls_debug", test_path, flags)
    #         release_vs_debug_results.append(speed_diff)
    #         print()
    
    print(f"{Colors.MAGENTA}Part 2: ft_ls vs system ls comparison{Colors.NC}")
    print()
    
    # Then compare ft_ls vs system ls
    for test_path in TEST_CASES:
        for flags in FLAG_COMBINATIONS:
            print(f"{Colors.YELLOW}=== Testing: {flags} {test_path} ==={Colors.NC}")
            speed_diff, output_match = compare_commands("./ft_ls", "ls", "ft_ls", "ls", test_path, flags)
            ft_ls_vs_ls_perf_results.append(speed_diff)
            
            if output_match is not None:
                total_output_tests += 1
                if output_match:
                    output_match_count += 1
            
            print()
    
    # Summary
    print(f"{Colors.YELLOW}=========================================={Colors.NC}")
    print(f"{Colors.YELLOW}              Summary Results              {Colors.NC}")
    print(f"{Colors.YELLOW}=========================================={Colors.NC}")
    
    if release_vs_debug_results:
        avg_release_vs_debug = sum(release_vs_debug_results) / len(release_vs_debug_results)
        if avg_release_vs_debug > 0:
            print(f"Release is {avg_release_vs_debug:.2f}% faster than debug on average")
        else:
            print(f"Release is {abs(avg_release_vs_debug):.2f}% slower than debug on average")
    
    if ft_ls_vs_ls_perf_results:
        avg_ft_ls_vs_ls = sum(ft_ls_vs_ls_perf_results) / len(ft_ls_vs_ls_perf_results)
        if avg_ft_ls_vs_ls > 0:
            print(f"ft_ls is {avg_ft_ls_vs_ls:.2f}% faster than system ls on average")
        else:
            print(f"ft_ls is {abs(avg_ft_ls_vs_ls):.2f}% slower than system ls on average")
    
    if total_output_tests > 0:
        match_percentage = (output_match_count / total_output_tests) * 100
        print(f"Output correctness: {output_match_count}/{total_output_tests} tests pass ({match_percentage:.1f}%)")
    
    print(f"{Colors.YELLOW}=========================================={Colors.NC}")
    print(f"{Colors.GREEN}Benchmark and testing completed!{Colors.NC}")

if __name__ == "__main__":
    main()