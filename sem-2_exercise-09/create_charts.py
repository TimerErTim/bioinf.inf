import pandas as pd
import matplotlib.pyplot as plt
import os

# Get the absolute path of the script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))

# Define paths relative to the script's directory
output_dir = os.path.join(script_dir, "out")
assets_dir = os.path.join(script_dir, "assets")
basic_operations_file = os.path.join(output_dir, "basic_operations_timing.csv")
binary_search_file = os.path.join(output_dir, "binary_search_analysis.csv")

# Create assets directory if it doesn't exist
os.makedirs(assets_dir, exist_ok=True)

def plot_basic_operations():
    """
    Reads the basic operations timing data and creates a bar chart.
    """
    try:
        df = pd.read_csv(basic_operations_file)
        
        plt.figure(figsize=(10, 6))
        plt.bar(df["Operation"], df["Time_ns"], color="skyblue")
        plt.xlabel("Operation")
        plt.ylabel("Time (ns)")
        plt.title("Laufzeiten der Basisoperationen")
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        
        # Save the plot
        save_path = os.path.join(assets_dir, "basic_operations_chart.png")
        plt.savefig(save_path)
        plt.close()
        
        print(f"Chart saved to {save_path}")
        
    except FileNotFoundError:
        print(f"Error: {basic_operations_file} not found.")
    except Exception as e:
        print(f"An error occurred while plotting basic operations: {e}")

def plot_binary_search_analysis():
    """
    Reads the binary search analysis data and creates line charts.
    """
    try:
        df = pd.read_csv(binary_search_file)

        # Pivot the table to get algorithms and scenarios as columns
        pivot_df = df.pivot_table(index='Size', columns=['Algorithm', 'Scenario'], values=['Measured_ns', 'Theoretical_ns'])
        
        # Plot for value found
        plt.figure(figsize=(12, 7))
        
        for i in range(1, 4):
            algo = f"V{i}"
            plt.plot(pivot_df.index, pivot_df[('Measured_ns', algo, 'Found')], marker='o', linestyle='-', label=f"Gemessen {algo}")
            plt.plot(pivot_df.index, pivot_df[('Theoretical_ns', algo, 'Found')], marker='x', linestyle='--', label=f"Berechnet {algo}")
            
        plt.xlabel("Array-Größe")
        plt.ylabel("Zeit (ns)")
        plt.title("Binäre Suche: Wert im Array gefunden")
        plt.legend()
        plt.grid(True)
        
        # Save the plot
        save_path = os.path.join(assets_dir, "binary_search_found_chart.png")
        plt.savefig(save_path)
        plt.close()
        
        print(f"Chart saved to {save_path}")
        
        # Plot for value not found
        plt.figure(figsize=(12, 7))
        
        for i in range(1, 4):
            algo = f"V{i}"
            plt.plot(pivot_df.index, pivot_df[('Measured_ns', algo, 'NotFound')], marker='o', linestyle='-', label=f"Gemessen {algo}")
            plt.plot(pivot_df.index, pivot_df[('Theoretical_ns', algo, 'NotFound')], marker='x', linestyle='--', label=f"Berechnet {algo}")
            
        plt.xlabel("Array-Größe")
        plt.ylabel("Zeit (ns)")
        plt.title("Binäre Suche: Wert nicht im Array gefunden")
        plt.legend()
        plt.grid(True)
        
        # Save the plot
        save_path = os.path.join(assets_dir, "binary_search_not_found_chart.png")
        plt.savefig(save_path)
        plt.close()
        
        print(f"Chart saved to {save_path}")
        
    except FileNotFoundError:
        print(f"Error: {binary_search_file} not found.")
    except Exception as e:
        print(f"An error occurred while plotting binary search analysis: {e}")

if __name__ == "__main__":
    plot_basic_operations()
    plot_binary_search_analysis() 