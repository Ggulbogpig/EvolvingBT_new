# import os
# import json
# import torch
# from torch import nn
# from torch.optim import Adam

# ACTION_LIST = ["Safe", "Runner", "Attack"]

# # ACTION_LIST = ["SafeMove", "RushMove", "FightBack"]




# class SoftmaxIRL:
#     def __init__(self, K, lr=0.05, device="cpu"):
#         self.K = K
#         self.device = device

#         init_w = torch.ones(K, dtype=torch.float32) / K
#         self.w_raw = nn.Parameter(init_w.clone())

#         self.optimizer = Adam([self.w_raw], lr=lr)

#     def project_simplex(self, v):
#         u, _ = torch.sort(v, descending=True)
#         cssv = torch.cumsum(u, dim=0)
#         rho = torch.nonzero(u * torch.arange(1, len(u)+1, device=v.device) > (cssv - 1), as_tuple=False)
#         rho = rho[-1, 0].item()
#         theta = (cssv[rho] - 1) / float(rho + 1)
#         return torch.clamp(v - theta, min=0.0)

#     # ---------------------------------------------------------
#     # ★ 여러 JSON 파일 읽기 기능 추가
#     # ---------------------------------------------------------
#     def load_multiple_demos(self, folder_path):


#         ACTION_LIST = ["Safe", "Runner", "Attack"]   # Unreal에서 쓰는 3개 액션 이름
#         sub_rewards_list = []
#         chosen_indices = []

#         for file in os.listdir(folder_path):
#             if not file.endswith(".json"):
#                 continue

#             full_path = os.path.join(folder_path, file)
#             data = json.load(open(full_path, "r"))

#         # UE JSON = { "trajectories": [...] }
#             steps = data["trajectories"]

#             for step in steps:  # ← continue는 반드시 이 루프 안에서만 사용 가능!
#                 action = step["action"]

#             # Idle, Patrol, 기타 행동은 모두 Skip
#                 if action not in ACTION_LIST:
#                     continue  # ← 여기는 반복문 inside라 OK!!!

#                 action_idx = ACTION_LIST.index(action)

#             # sub_rewards: (3 × K)
#                 sr = step["sub_rewards"]
#                 sr_tensor = torch.tensor(sr, dtype=torch.float32)

#                 sub_rewards_list.append(sr_tensor)
#                 chosen_indices.append(action_idx)

#     # K는 sub_reward의 column size (예: 6)
#         K = sub_rewards_list[0].shape[1] if len(sub_rewards_list) > 0 else 0

#         return sub_rewards_list, chosen_indices, K

   


#     def compute_loss(self, sub_rewards_list, chosen_indices):
#         w = self.project_simplex(self.w_raw)
#         total_log_prob = 0.0
#         total_steps = 0

#         for sub_rewards, chosen in zip(sub_rewards_list, chosen_indices):
#             q = sub_rewards @ w
#             log_probs = torch.log_softmax(q, dim=0)

#             total_log_prob += log_probs[chosen]
#             total_steps += 1

#         return -total_log_prob / total_steps


#     def fit(self, sub_rewards_list, chosen_indices, num_steps=500, print_every=50):
#         for step in range(1, num_steps+1):
#             self.optimizer.zero_grad()
#             loss = self.compute_loss(sub_rewards_list, chosen_indices)
#             loss.backward()
#             self.optimizer.step()

#             if step % print_every == 0:
#                 with torch.no_grad():
#                     w_proj = self.project_simplex(self.w_raw)
#                 print(f"[SoftmaxIRL] step={step}, loss={loss.item():.4f}, w={w_proj.tolist()}")

#         with torch.no_grad():
#             return self.project_simplex(self.w_raw)



# # ---------------------------------------------------------
# # ★ main(): 폴더에서 여러 파일 읽어서 IRL 실행
# # ---------------------------------------------------------
# def main():
#     folder = "./Trajectory"  # 원하는 폴더 이름

#     irl_temp = SoftmaxIRL(K=6)   # 임시 K, 실제 K는 JSON에서 다시 불러옴
#     sub_rewards_list, chosen_indices, K = irl_temp.load_multiple_demos(folder)

#     irl = SoftmaxIRL(K=K, lr=0.08)
#     w = irl.fit(sub_rewards_list, chosen_indices, num_steps=5000)

#     result = {
#         "objective_ids": irl.objective_ids,
#         "weights": w.tolist()
#     }
#     with open("estimated_weights_softmax.json", "w") as f:
#         json.dump(result, f, indent=2)

#     print("[SoftmaxIRL] Final weights saved to estimated_weights_softmax.json")


# if __name__ == "__main__":
#     main()

import os
import json
import torch
from torch import nn
from torch.optim import Adam

# Unreal에서 실제로 사용할 3개 액션
ACTION_LIST = ["Safe", "Runner", "Attack"]

# 목적 함수 이름들 (K = 6 기준)
OBJECTIVE_IDS = [
    "DistanceSafety",
    "GoalProgress",
    "VisibilityRisk",
    "HealthSurvival",
    "ItemValue",
    "TimeEfficiency"
]


class SoftmaxIRL:
    def __init__(self, K, lr=0.05, device="cpu"):
        self.K = K
        self.device = device

        init_w = torch.ones(K, dtype=torch.float32) / K
        self.w_raw = nn.Parameter(init_w.clone())

        self.optimizer = Adam([self.w_raw], lr=lr)

    def project_simplex(self, v):
        u, _ = torch.sort(v, descending=True)
        cssv = torch.cumsum(u, dim=0)
        rho = torch.nonzero(u * torch.arange(1, len(u)+1, device=v.device) > (cssv - 1),
                            as_tuple=False)[-1, 0].item()
        theta = (cssv[rho] - 1) / float(rho + 1)
        return torch.clamp(v - theta, min=0.0)

    # ---------------------------------------------------------
    # 여러 JSON 읽기
    # ---------------------------------------------------------
    def load_multiple_demos(self, folder_path):

        sub_rewards_list = []
        chosen_indices = []

        for file in os.listdir(folder_path):
            if not file.endswith(".json"):
                continue

            full_path = os.path.join(folder_path, file)
            data = json.load(open(full_path, "r"))

            if "trajectories" not in data:
                continue

            steps = data["trajectories"]

            for step in steps:

                action = step["action"]

                # Idle, Patrol 등 스킵
                if action not in ACTION_LIST:
                    continue

                action_idx = ACTION_LIST.index(action)

                sr = step["sub_rewards"]     # (3 x 6)
                sr_tensor = torch.tensor(sr, dtype=torch.float32)

                sub_rewards_list.append(sr_tensor)
                chosen_indices.append(action_idx)

        if len(sub_rewards_list) == 0:
            raise ValueError("ERROR: JSON 데이터에서 유효한 IRL 샘플이 하나도 없습니다!")

        # sub_reward의 column = feature 수
        K = sub_rewards_list[0].shape[1]

        return sub_rewards_list, chosen_indices, K


    # ---------------------------------------------------------
    # Loss 계산
    # ---------------------------------------------------------
    def compute_loss(self, sub_rewards_list, chosen_indices):
        w = self.project_simplex(self.w_raw)

        total_log_prob = 0.0
        total_steps = 0

        for sub_rewards, chosen in zip(sub_rewards_list, chosen_indices):
            q = sub_rewards @ w
            log_probs = torch.log_softmax(q, dim=0)
            total_log_prob += log_probs[chosen]
            total_steps += 1

        return -total_log_prob / total_steps


    # ---------------------------------------------------------
    # 최적화 루프
    # ---------------------------------------------------------
    def fit(self, sub_rewards_list, chosen_indices, num_steps=500, print_every=50):
        for step in range(1, num_steps + 1):
            self.optimizer.zero_grad()

            loss = self.compute_loss(sub_rewards_list, chosen_indices)
            loss.backward()
            self.optimizer.step()

            if step % print_every == 0:
                w_proj = self.project_simplex(self.w_raw).detach()
                print(f"[SoftmaxIRL] step={step}, loss={loss.item():.4f}, w={w_proj.tolist()}")

        return self.project_simplex(self.w_raw).detach()



# ---------------------------------------------------------
# MAIN
# ---------------------------------------------------------
def main():
    folder = "./Trajectory"

    irl_temp = SoftmaxIRL(K=6)   # 임시 객체
    sub_rewards_list, chosen_indices, K = irl_temp.load_multiple_demos(folder)

    irl = SoftmaxIRL(K=K, lr=0.05)
    w = irl.fit(sub_rewards_list, chosen_indices, num_steps=5000)

    # JSON으로 저장
    result = {
        "objective_ids": OBJECTIVE_IDS,
        "weights": w.tolist()
    }

    with open("estimated_weights_softmax.json", "w") as f:
        json.dump(result, f, indent=2)

    print("[SoftmaxIRL] Final weights saved to estimated_weights_softmax.json")


if __name__ == "__main__":
    main()
