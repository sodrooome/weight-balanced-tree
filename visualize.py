import matplotlib.pyplot as plt

categories = ["Constant Time", "Linear Time (w/ Features Classification)"]
eval_metrics = ["Precision", "Recall", "F1 Score", "Response Time"]

# all of these values are constant data that manually
# retrieved after benchmarking process, for now just
# input it one-by-one
avg_constant_values = [100, 100, 100, 0.08] 
avg_linear_values = [23.08, 100, 37.50, 1.68]

x = range(len(eval_metrics))
width = 0.35

figure, ax = plt.subplots()
figure_1 = ax.bar(x, avg_constant_values, width, label="Constant Time")
figure_2 = ax.bar(
    [index + 1 for index in ax],
    avg_linear_values,
    width,
    label="Linear Time (w/Features Classification)",
)

ax.set_ylabel("Percentage (%)")
ax.set_title("Weighted Binary Tree evaluation metrics")
ax.set_xticks([index + 1 / 2 for index in ax])
ax.set_xticklabels(eval_metrics)
ax.legend()


def label_visualization(figures):
    for figure in figures:
        height = figure.get_height()
        ax.annotate(
            {}.format(height),
            xy=(figure.get_x() + figure.get_width() / 2, height),
            xytext=(0, 3),
            textcoords="Points",
            ha="center",
            va="bottom",
        )


label_visualization(figure_1)
label_visualization(figure_2)

plt.tight_layout()
plt.show()
