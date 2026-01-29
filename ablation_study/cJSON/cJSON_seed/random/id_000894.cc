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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const double val1 = 123.5;
    const double val2 = 6.5;
    cJSON *root = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *samples = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *avg = (cJSON *)0;
    cJSON *retrieved_metrics = (cJSON *)0;
    cJSON *retrieved_samples = (cJSON *)0;
    cJSON *retrieved_avg = (cJSON *)0;
    cJSON_bool added_samples = (cJSON_bool)0;
    cJSON_bool added_avg = (cJSON_bool)0;
    cJSON_bool added_metrics = (cJSON_bool)0;
    cJSON_bool is_array_flag = (cJSON_bool)0;
    double avg_value = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create objects and numeric items
    root = cJSON_CreateObject();
    metrics = cJSON_CreateObject();
    samples = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(val1);
    num2 = cJSON_CreateNumber(val2);
    avg = cJSON_CreateNumber((val1 + val2) / 2.0);

    // step 3: Configure - assemble JSON structure
    added_samples = cJSON_AddItemToArray(samples, num1);
    added_samples = (cJSON_bool)(added_samples + (int)cJSON_AddItemToArray(samples, num2));
    added_samples = (cJSON_bool)(added_samples + (int)cJSON_AddItemToObject(metrics, "samples", samples));
    added_avg = cJSON_AddItemToObject(metrics, "avg", avg);
    added_metrics = cJSON_AddItemToObject(root, "metrics", metrics);

    // step 4: Operate - retrieve items and extract values
    retrieved_metrics = cJSON_GetObjectItem(root, "metrics");
    retrieved_samples = cJSON_GetObjectItem(retrieved_metrics, "samples");
    is_array_flag = cJSON_IsArray(retrieved_samples);
    retrieved_avg = cJSON_GetObjectItem(retrieved_metrics, "avg");
    avg_value = cJSON_GetNumberValue(retrieved_avg);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a simple score to ensure data flowed
    validation_score = (int)(root != (cJSON *)0)
                     + (int)is_array_flag
                     + (int)(retrieved_avg != (cJSON *)0)
                     + (int)(printed != (char *)0)
                     + (int)(avg_value == ((val1 + val2) / 2.0));

    // step 6: Cleanup
    if (printed != (char *)0) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}