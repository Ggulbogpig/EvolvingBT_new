


import os
import json
import torch
from softmax_irl import SoftmaxIRL


def load_all_json(folder_path):
    reward_vectors = []

    # 폴더 안의 모든 json 파일 읽기
    for file in os.listdir(folder_path):
        if file.endswith(".json"):
            full_path = os.path.join(folder_path, file)
            print(f"[LOAD] {full_path}")

            with open(full_path, "r") as f:
                data = json.load(f)

            traj = data["trajectories"]

            for step in traj:
                reward_vectors.append(step["sub_rewards"])

    print(f"[INFO] Total samples loaded = {len(reward_vectors)}")
    return torch.tensor(reward_vectors, dtype=torch.float32)


def main():
    folder = "./Trajectory"   # Unreal JSON 저장 위치
    reward_vectors = load_all_json(folder)

    print("[INFO] Reward vector shape:", reward_vectors.shape)

    irl = SoftmaxIRL(feature_dim=reward_vectors.shape[1])
    final_w = irl.fit(reward_vectors, num_steps=5000)

    print("\n=== FINAL W ===")
    print(final_w.tolist())


if __name__ == "__main__":
    main()
