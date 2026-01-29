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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *score = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "score", score);

    // step 2: Configure
    cJSON *values_ref = cJSON_GetObjectItem(root, "values");
    cJSON *second_item = cJSON_GetArrayItem(values_ref, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    double new_score = cJSON_SetNumberHelper(score, second_val * 2.0);

    // step 3: Operate and Validate
    double score_val = cJSON_GetNumberValue(score);
    cJSON_AddNumberToObject(root, "score_snapshot", score_val);
    cJSON_AddNumberToObject(root, "computed_from_set_helper", new_score);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}