import matplotlib.pyplot as plt

# Veriler (Senin projelerindeki yaklaşık değerler)
labels = ['Single-Threaded', '8-Threaded (M-series)']
processing_time = [20, 8]  # Milisaniye cinsinden (Düşük olan daha iyi)

# Grafik Renkleri (Senin istediğin gibi Mavi ve Turuncu)
colors = ['#4A90E2', '#F5A623']

plt.figure(figsize=(8, 6))
bars = plt.bar(labels, processing_time, color=colors, width=0.6)

# Estetik Dokunuşlar
plt.ylabel('Average Processing Time (ms)', fontsize=12)
plt.title('Performance Comparison: Single vs Multi-Threading', fontsize=14, fontweight='bold')
plt.ylim(0, 25)

# Çubukların üzerine değerleri yazalım
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 0.5, f'{yval}ms', ha='center', va='bottom', fontweight='bold')

plt.grid(axis='y', linestyle='--', alpha=0.7)

# Kaydet
plt.savefig('performance_graph.png', dpi=300, bbox_inches='tight')
print("Grafik 'performance_graph.png' olarak kaydedildi!")
