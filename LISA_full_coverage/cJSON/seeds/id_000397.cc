#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *num_answer = cJSON_CreateNumber(42.0);
    cJSON *num_pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(metrics, "answer", num_answer);
    cJSON_AddItemToObject(metrics, "pi", num_pi);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *temp = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "temp", temp);

    /* create a safe copy of metrics to avoid double-free issues when deleting root */
    cJSON *metrics_ref = cJSON_Duplicate(metrics, 1);
    cJSON_AddItemToObject(root, "metrics_ref", metrics_ref);

    cJSON *pi_lookup = cJSON_GetObjectItemCaseSensitive(metrics, "pi");
    double pi_value = cJSON_GetNumberValue(pi_lookup);
    cJSON *answer_lookup = cJSON_GetObjectItemCaseSensitive(metrics, "answer");
    double answer_value = cJSON_GetNumberValue(answer_lookup);

    double combined = pi_value + answer_value;
    cJSON *combined_item = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "combined", combined_item);
    char *printed_before = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(printed_before);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    char *printed_after = cJSON_PrintBuffered(root, 128, 0);
    cJSON_free(printed_after);

    cJSON_Delete(root);
    return 66;
}