
# softmax_irl_new.py
import json
import torch
from torch import nn
from torch.optim import Adam

class SoftmaxIRL:
    def __init__(self, K, lr=0.01, device="cpu"):
        self.K = K
        self.device = device
        
        init_w = torch.ones(K, dtype=torch.float32) / K
        self.w_raw = nn.Parameter(init_w.clone())

        self.optimizer = Adam([self.w_raw], lr=lr)

    def project_simplex(self, v):
        u, _ = torch.sort(v, descending=True)
        cssv = torch.cumsum(u, dim=0)
        rho = torch.nonzero(u * torch.arange(1, len(u)+1, device=v.device) > (cssv - 1), as_tuple=False)
        rho = rho[-1, 0].item()
        theta = (cssv[rho] - 1) / float(rho + 1)
        w = torch.clamp(v - theta, min=0.0)
        return w

    def load_demo(self, json_path):
        data = json.load(open(json_path, "r"))
        traj = data["trajectories"]

        reward_vectors = []

        for step in traj:
            r = torch.tensor(step["sub_rewards"], dtype=torch.float32)  # (K,)
            reward_vectors.append(r)

        return reward_vectors

    def compute_loss(self, reward_vectors):
        w = self.project_simplex(self.w_raw)  # (K,)
        total = 0

        for r in reward_vectors:
            # 목표는 w·r 를 최대화 → loss = - (w·r)
            total += - torch.dot(w, r)

        return total / len(reward_vectors)

    def fit(self, reward_vectors, num_steps=2000, print_every=200):
        for step in range(1, num_steps+1):
            self.optimizer.zero_grad()

            loss = self.compute_loss(reward_vectors)

            loss.backward()
            self.optimizer.step()

            if step % print_every == 0:
                with torch.no_grad():
                    w_proj = self.project_simplex(self.w_raw)
                print(f"[IRL] step={step}, loss={loss.item():.4f}, w={w_proj.tolist()}")

        with torch.no_grad():
            final_w = self.project_simplex(self.w_raw)
        return final_w


def main():
<<<<<<< Updated upstream
    import argparse

    # parser = argparse.ArgumentParser()
    # parser.add_argument("--demo_json", type=str, required=True)
    # parser.add_argument("--lr", type=float, default=0.05)
    # parser.add_argument("--steps", type=int, default=500)
    # parser.add_argument("--out_json", type=str, default="estimated_weights_softmax.json")
    # args = parser.parse_args()

    # 데모 로드
    # tmp = json.load(open(args.trajectory, "r"))
    demo_json = "trajectory.json"
=======
    demo = "IRL_Run_Safe.json"
>>>>>>> Stashed changes

    reward_dim = 7   # 6차원 reward
    lr = 0.05
<<<<<<< Updated upstream
    steps=1000
    out_json = "estimated_weights_softmax.json"
=======
    steps = 5000
>>>>>>> Stashed changes

    irl = SoftmaxIRL(K=reward_dim, lr=lr)
    reward_vectors = irl.load_demo("IRLRecord_20251210_085543.json")

    final_w = irl.fit(reward_vectors, num_steps=steps)

    print("\n=== FINAL W ===")
    print(final_w.tolist())


