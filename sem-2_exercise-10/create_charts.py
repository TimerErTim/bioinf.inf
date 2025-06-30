import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.colorbar import Colorbar
import numpy as np
import os
# The linter may not resolve this import if 'hilbertcurve' is not in the environment.
# Please ensure it is installed: pip install hilbertcurve
from hilbertcurve.hilbertcurve import HilbertCurve
import warnings

# Suppress future warnings from seaborn
warnings.simplefilter(action='ignore', category=FutureWarning)

# Define relative paths
OUT_DIR = 'out'
ASSETS_DIR = 'assets'
HEATMAP_DIR = os.path.join(ASSETS_DIR, 'heatmaps')

# Ensure the assets directories exist
os.makedirs(ASSETS_DIR, exist_ok=True)
os.makedirs(HEATMAP_DIR, exist_ok=True)


def parse_filename(filename):
    """Robustly parses the filename to extract test parameters."""
    filename = filename.replace('.csv', '')
    
    known_cases = ['average', 'worst', 'best']
    known_algorithms = ['brute_force', 'boyer_moore', 'kmp']
    known_sizes = ['large', 'small']

    case = next((c for c in known_cases if filename.endswith(f'_{c}')), None)
    if case:
        filename = filename[:-len(case) - 1] # Remove '_case'

    algorithm = next((a for a in known_algorithms if filename.endswith(f'_{a}')), None)
    if algorithm:
        filename = filename[:-len(algorithm) - 1] # Remove '_algorithm'

    size = next((s for s in known_sizes if filename.endswith(f'_{s}')), None)
    if size:
        filename = filename[:-len(size) - 1] # Remove '_size'
    
    alphabet = filename

    if not all([alphabet, size, algorithm, case]):
        print(f"Warning: Could not fully parse filename: {filename}.csv")
        return None

    return alphabet, size, algorithm, case


def resample_preserving_distribution(data, target_length):
    """
    Resamples the input data to the specified target length
    while preserving the distribution of the original data.

    Parameters:
        data (list or np.ndarray): Original data points.
        target_length (int): Desired number of output points.

    Returns:
        np.ndarray: Resampled data of length target_length.
    """
    if target_length <= 0:
        raise ValueError("Target length must be a positive integer.")
    if len(data) == 0:
        return np.array([])

    # Sort data to get the empirical CDF
    sorted_data = np.sort(data)
    n = len(sorted_data)

    # Compute original CDF values (quantiles)
    original_cdf = np.linspace(0, 1, n)

    # Target quantiles to interpolate
    target_cdf = np.linspace(0, 1, target_length)

    # Interpolate to get values corresponding to target quantiles
    resampled_data = np.interp(target_cdf, original_cdf, data)

    return resampled_data

def create_comparison_chart(summary_df):
    """Generates a bar chart comparing total comparisons for each algorithm."""
    print("Generating comparison bar chart...")
    
    df = summary_df[summary_df['case'] == 'average'].copy()

    g = sns.catplot(
        data=df,
        x='size',
        y='total_comparisons',
        hue='algorithm',
        col='alphabet',
        kind='bar',
        height=4, aspect=0.8,
        sharey=False, col_wrap=2
    )
    g.set_axis_labels("", "Total Comparisons (Average Case)")
    g.set_titles("Alphabet: {col_name}")
    g.fig.suptitle('Average Case Character Comparisons by Algorithm and Alphabet', y=1.03)
    
    for ax in g.axes.flat:
        if ax:
            ax.set_yscale('log')
            ax.grid(True, which='both', linestyle='--', linewidth=0.5)

    # Move the legend to the bottom of the figure
    sns.move_legend(
        g, "lower center",
        bbox_to_anchor=(.5, 0.00),
        ncol=3,
        title=None,
        frameon=False,
    )
    # Adjust subplot params to make room for the legend
    g.fig.subplots_adjust(bottom=0.1)
    
    filename = os.path.join(ASSETS_DIR, 'comparison_chart.png')
    plt.savefig(filename, bbox_inches='tight')
    plt.close()
    print(f"Saved chart to {filename}")


def create_alphabet_size_impact_chart(summary_df):
    """Generates a line plot showing the impact of alphabet size."""
    print("Generating alphabet size impact chart...")
    
    df = summary_df[(summary_df['size'] == 'large') & (summary_df['case'] == 'average')].copy()
    
    alphabet_sizes = {'binary': 2, 'dna': 4, 'amino_acid': 23, 'ascii': 256}
    df['alphabet_size'] = df['alphabet'].map(alphabet_sizes)
    
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df, x='alphabet_size', y='total_comparisons', hue='algorithm', marker='o')
    
    plt.xscale('log')
    plt.yscale('log')
    plt.title('Algorithm Performance vs. Alphabet Size (Large Texts, Average Case)')
    plt.xlabel('Alphabet Size (log scale)')
    plt.ylabel('Total Comparisons (log scale)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title='Algorithm')
    
    filename = os.path.join(ASSETS_DIR, 'alphabet_size_impact.png')
    plt.savefig(filename)
    plt.close()
    print(f"Saved chart to {filename}")


def create_hilbert_heatmap(alphabet, size, algorithm, case):
    """Generates a stretched heatmap of text access patterns using a Hilbert curve."""
    test_name = f"{alphabet}_{size}_{algorithm}_{case}"
    print(f"Generating Hilbert curve heatmap for {test_name}...")
    
    csv_path = os.path.join(OUT_DIR, f"{test_name}.csv")
    if not os.path.exists(csv_path):
        print(f"CSV file not found, skipping: {csv_path}")
        return
        
    df = pd.read_csv(csv_path)
    df['total_comparisons'] = df['successful_comparisons'] + df['unsuccessful_comparisons']
    
    n_data_points = len(df)
    p = int(np.ceil(np.log2(n_data_points) / 2))
    p = max(p - 4, 1)
    side_length = 2**p
    n_curve_points = side_length**2
    
    curve = HilbertCurve(p, 2)
    
    # Stretch/compress the data points over the full length of the curve
    mapped_indices = np.linspace(0, n_curve_points - 1, n_curve_points, dtype=int)
    coords = np.array(curve.points_from_distances(mapped_indices))
    
    image = np.zeros((side_length, side_length))
    comparisons = resample_preserving_distribution(df['total_comparisons'].to_numpy(), n_curve_points)
    # Quadratic scaling to highlight differences
    image[coords[:, 0], coords[:, 1]] = np.array(comparisons)
    
    plt.figure(figsize=(10, 10))
    heatmap = sns.heatmap(image, square=True, cmap='mako', cbar_kws={'label': 'Total Comparisons', "shrink": 0.8})
    plt.title(f'Heatmap of Text Access\n({algorithm} on {alphabet} {size} - {case} case)')
    plt.axis('off')

    # Add colorbar
    colorbar: Colorbar = heatmap.collections[0].colorbar  # type: ignore
    colorbar.set_label("Total Comparisons", fontsize=10)
    
    filename = os.path.join(HEATMAP_DIR, f'{test_name}_heatmap.png')
    plt.savefig(filename, bbox_inches='tight')
    plt.close()
    print(f"Saved heatmap to {filename}")


def main():
    """Main function to generate all charts."""
    if not os.path.isdir(OUT_DIR):
        print(f"Error: Output directory '{OUT_DIR}' not found.")
        return

    all_files = [f for f in os.listdir(OUT_DIR) if f.endswith('.csv')]
    if not all_files:
        print(f"No CSV files found in '{OUT_DIR}'.")
        return

    summary_data = []
    for filename in all_files:
        parsed = parse_filename(filename)
        if not parsed:
            continue
        
        alphabet, size, algorithm, case = parsed
        
        df = pd.read_csv(os.path.join(OUT_DIR, filename))
        total_comparisons = df['successful_comparisons'].sum() + df['unsuccessful_comparisons'].sum()
        
        summary_data.append({
            'alphabet': alphabet, 'size': size, 'algorithm': algorithm,
            'case': case, 'total_comparisons': total_comparisons
        })
        
    summary_df = pd.DataFrame(summary_data)
    
    create_comparison_chart(summary_df)
    create_alphabet_size_impact_chart(summary_df)
    
    print("\nGenerating heatmaps for all large scenarios (worst and average)...")
    large_cases_df = summary_df[
        (summary_df['size'] == 'large') & 
        (summary_df['case'].isin(['worst', 'average']))
    ]
    for index, row in large_cases_df.iterrows():
        create_hilbert_heatmap(row['alphabet'], row['size'], row['algorithm'], row['case'])


if __name__ == '__main__':
    main()
