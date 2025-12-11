import os
import json
import torch
from softmax_irl import SoftmaxIRL   # ← 반드시 new 버전 사용!

def load_all_json(folder_path):
    reward_vectors = []
    reward_dim = None

    for file in os.listdir(folder_path):
        if not file.endswith(".json"):
            continue

        full_path = os.path.join(folder_path, file)
        print(f"[LOAD] {full_path}")

        try:
            with open(full_path, "r") as f:
                data = json.load(f)
        except:
            print(f"[SKIP] Failed to read {file}")
            continue

        if "trajectories" not in data:
            print(f"[SKIP] No trajectories in {file}")
            continue

        for step in data["trajectories"]:
            if "sub_rewards" not in step:
                continue

            r = step["sub_rewards"]

            if reward_dim is None:
                reward_dim = len(r)

            reward_vectors.append(r)

    print(f"[INFO] Total samples = {len(reward_vectors)}")
    print(f"[INFO] Reward dimension = {reward_dim}")

    return torch.tensor(reward_vectors, dtype=torch.float32), reward_dim


def main():
    folder = "./Trajectory"

    reward_vectors, reward_dim = load_all_json(folder)

    if reward_dim is None:
        print("[ERROR] No valid demo found!")
        return

    irl = SoftmaxIRL(K=reward_dim, lr=0.01)
    final_w = irl.fit(reward_vectors, num_steps=5000, print_every=200)

    print("\n=== FINAL W ===")
    print(final_w.tolist())


if __name__ == "__main__":
    main()
