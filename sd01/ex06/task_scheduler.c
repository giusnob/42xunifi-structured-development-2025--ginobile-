#include "task_scheduler.h"

struct ScheduleResult *schedule_tasks(struct TaskList *tasks)
{
    struct TaskProfile *profile = profile_tasks(tasks);
    if (!profile)
        return (NULL);

    struct PriorityMap *priorities = compute_priorities_mock(profile);
    if (!priorities)
    {
        free_task_profile(profile);
        return (NULL);
    }

    int core_count = get_available_core_count();
    if (core_count <= 0)
    {
        free_priority_map(priorities);
        free_task_profile(profile);
        return (NULL);
    }

    struct ScheduleResult *result = create_schedule_result(core_count);
    if (!result)
    {
        free_priority_map(priorities);
        free_task_profile(profile);
        return (NULL);
    }

    int core_id = 0;
    while (core_id < core_count)
    {
        int task_id = select_best_task(profile);
        if (task_id != -1)
            update_schedule_entry(result, core_id, task_id);
        core_id++;
    }

    free_priority_map(priorities);
    free_task_profile(profile);

    return (result);
}
