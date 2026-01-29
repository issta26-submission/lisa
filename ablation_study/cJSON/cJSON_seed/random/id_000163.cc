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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON *retrieved_n1 = (cJSON *)0;
    cJSON *retrieved_n2 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON *meta_is_object_flag = (cJSON *)0;
    char *json_text = (char *)0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(0.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "n1", n1);
    cJSON_AddItemToObject(meta, "n2", n2);

    // step 4: Operate
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    meta_is_object_flag = cJSON_CreateNumber((double)cJSON_IsObject(retrieved_meta));
    retrieved_n1 = cJSON_GetObjectItem(retrieved_meta, "n1");
    retrieved_n2 = cJSON_GetObjectItem(retrieved_meta, "n2");
    v1 = cJSON_GetNumberValue(retrieved_n1);
    v2 = cJSON_GetNumberValue(retrieved_n2);
    sum = v1 + v2;
    sum_item = cJSON_CreateNumber(sum);

    // step 5: Validate
    cJSON_AddItemToObject(root, "meta_is_object", meta_is_object_flag);
    cJSON_AddItemToObject(root, "sum", sum_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}