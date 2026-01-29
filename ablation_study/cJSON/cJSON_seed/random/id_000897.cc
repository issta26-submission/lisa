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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"metrics\": [1, 2.5, 3]}";
    cJSON *parsed = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *summary = (cJSON *)0;
    cJSON *avg_item = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double avg = 0.0;
    cJSON_bool is_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate - extract array and numeric values
    metrics = cJSON_GetObjectItem(parsed, "metrics");
    is_array = cJSON_IsArray(metrics);
    first = cJSON_GetArrayItem(metrics, 0);
    second = cJSON_GetArrayItem(metrics, 1);
    v0 = cJSON_GetNumberValue(first);
    v1 = cJSON_GetNumberValue(second);
    avg = (v0 + v1) / 2.0;

    // step 4: Configure - build summary object and attach parsed content
    summary = cJSON_CreateObject();
    avg_item = cJSON_CreateNumber(avg);
    cJSON_AddItemToObject(summary, "average_first_two", avg_item);
    cJSON_AddItemToObject(summary, "original", parsed);

    // step 5: Validate - print and re-read value to ensure flow
    printed = cJSON_Print(summary);
    validation_score = (int)is_array + (int)(printed != (char *)0) + (int)(avg_item != (cJSON *)0) + (int)(cJSON_GetNumberValue(cJSON_GetObjectItem(summary, "average_first_two")) == avg);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(summary);

    // API sequence test completed successfully
    return 66;
}