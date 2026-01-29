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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *retr_pi = (cJSON *)0;
    cJSON *retr_added = (cJSON *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool added_ok = 0;
    double pi_value = 0.0;
    double added_value = 0.0;
    int values_size = 0;

    // step 2: Initialize
    const char *json = "{\"metrics\":{\"values\":[1.0,2.0],\"pi\":3.14159},\"tag\":\"test\"}";
    root = cJSON_ParseWithOpts(json, &parse_end, 1);
    new_num = cJSON_CreateNumber(42.42);

    // step 3: Configure
    metrics = cJSON_GetObjectItem(root, "metrics");
    values = cJSON_GetObjectItem(metrics, "values");
    added_ok = cJSON_AddItemToArray(values, new_num);

    // step 4: Operate
    retr_pi = cJSON_GetObjectItemCaseSensitive(metrics, "pi");
    pi_value = cJSON_GetNumberValue(retr_pi);
    values_size = cJSON_GetArraySize(values);
    retr_added = cJSON_GetArrayItem(values, values_size - 1);
    added_value = cJSON_GetNumberValue(retr_added);

    // step 5: Validate
    (void)added_ok;
    (void)pi_value;
    (void)added_value;
    (void)parse_end;
    (void)metrics;
    (void)values_size;
    (void)retr_added;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}