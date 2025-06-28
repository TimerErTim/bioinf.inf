import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import os
from hilbertcurve.hilbertcurve import HilbertCurve
import warnings

# Suppress future warnings from seaborn
warnings.simplefilter(action='ignore', category=FutureWarning)

# Define relative paths
OUT_DIR = 'out'
ASSETS_DIR = 'assets'

# Ensure the assets directory exists
os.makedirs(ASSETS_DIR, exist_ok=True)


def create_comparison_chart(summary_df):
    """Generates a bar chart comparing total comparisons for each algorithm."""
    print("Generating comparison bar chart...")
    
    g = sns.catplot(
        data=summary_df,
        x='size',
        y='total_comparisons',
        hue='algorithm',
        col='alphabet',
        kind='bar',
        height=4,
        aspect=0.8,
        sharey=False,
        col_wrap=2
    )
    g.set_axis_labels("Test Size", "Total Comparisons")
    g.set_titles("Alphabet: {col_name}")
    g.fig.suptitle('Total Character Comparisons by Algorithm and Alphabet', y=1.03)
    
    # Use log scale for y-axis for better visibility
    for ax in g.axes.flat:
        ax.set_yscale('log')
        ax.grid(True, which='both', linestyle='--', linewidth=0.5)

    plt.tight_layout(rect=[0, 0, 1, 0.97])
    
    filename = os.path.join(ASSETS_DIR, 'comparison_chart.png')
    plt.savefig(filename)
    plt.close()
    print(f"Saved chart to {filename}")


def create_alphabet_size_impact_chart(summary_df):
    """Generates a line plot showing the impact of alphabet size."""
    print("Generating alphabet size impact chart...")
    
    # Filter for large test cases
    large_tests_df = summary_df[summary_df['size'] == 'large'].copy()
    
    alphabet_sizes = {
        'binary': 2,
        'dna': 4,
        'amino_acid': 23,
        'ascii': 256
    }
    large_tests_df['alphabet_size'] = large_tests_df['alphabet'].map(alphabet_sizes)
    
    plt.figure(figsize=(10, 6))
    sns.lineplot(
        data=large_tests_df,
        x='alphabet_size',
        y='total_comparisons',
        hue='algorithm',
        marker='o'
    )
    
    plt.xscale('log')
    plt.yscale('log')
    plt.title('Algorithm Performance vs. Alphabet Size (Large Texts)')
    plt.xlabel('Alphabet Size (log scale)')
    plt.ylabel('Total Comparisons (log scale)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title='Algorithm')
    
    filename = os.path.join(ASSETS_DIR, 'alphabet_size_impact.png')
    plt.savefig(filename)
    plt.close()
    print(f"Saved chart to {filename}")


def create_hilbert_heatmap(test_name, algorithm):
    """Generates a heatmap of text access patterns using a Hilbert curve."""
    print(f"Generating Hilbert curve heatmap for {test_name} - {algorithm}...")
    
    csv_path = os.path.join(OUT_DIR, f"{test_name}_{algorithm}.csv")
    if not os.path.exists(csv_path):
        print(f"CSV file not found, skipping: {csv_path}")
        return
        
    df = pd.read_csv(csv_path)
    df['total_comparisons'] = df['successful_comparisons'] + df['unsuccessful_comparisons']
    
    n = len(df)
    # The number of points for the curve must be a power of 2.
    # p is the number of iterations, creating a 2^p x 2^p grid.
    # The length of the curve is (2^p)^2 = 2^(2p). We need 2^(2p) >= n.
    p = int(np.ceil(np.log2(n) / 2))
    side_length = 2**p
    
    curve = HilbertCurve(p, 2)
    
    image = np.zeros((side_length, side_length))
    
    # Get coordinates for all points at once
    distances = np.arange(n)
    # Ensure coords is a numpy array for slicing
    coords = np.array(curve.points_from_distances(distances))
    
    # Use a log scale for color intensity for better visualization
    image[coords[:, 0], coords[:, 1]] = np.log1p(df['total_comparisons'])
    
    plt.figure(figsize=(8, 8))
    plt.imshow(image, cmap='viridis', origin='lower')
    plt.title(f'Heatmap of Text Access ({algorithm} on {test_name})')
    plt.axis('off') # No need for axis ticks
    
    # Save with a specific name for the documentation
    if algorithm == "brute_force":
        filename = os.path.join(ASSETS_DIR, 'comparison_heatmap.png')
    else:
        filename = os.path.join(ASSETS_DIR, f'comparison_heatmap_{algorithm}.png')
        
    plt.savefig(filename)
    plt.close()
    print(f"Saved heatmap to {filename}")


def main():
    """Main function to generate all charts."""
    if not os.path.isdir(OUT_DIR):
        print(f"Error: Output directory '{OUT_DIR}' not found. Please run the C++ program first.")
        return

    all_files = [f for f in os.listdir(OUT_DIR) if f.endswith('.csv')]
    
    if not all_files:
        print(f"No CSV files found in '{OUT_DIR}'.")
        return

    summary_data = []
    for filename in all_files:
        parts = filename.replace('.csv', '').rsplit('_', 2)
        test_name, size, algorithm = parts
        alphabet = test_name
        
        df = pd.read_csv(os.path.join(OUT_DIR, filename))
        total_comparisons = df['successful_comparisons'].sum() + df['unsuccessful_comparisons'].sum()
        
        summary_data.append({
            'alphabet': alphabet,
            'size': size,
            'algorithm': algorithm,
            'total_comparisons': total_comparisons
        })
        
    summary_df = pd.DataFrame(summary_data)
    
    # Generate charts
    create_comparison_chart(summary_df)
    create_alphabet_size_impact_chart(summary_df)
    
    # Generate heatmaps for the largest ASCII test case to show the difference
    create_hilbert_heatmap('ascii_large', 'brute_force')
    create_hilbert_heatmap('ascii_large', 'kmp')


if __name__ == '__main__':
    main()
