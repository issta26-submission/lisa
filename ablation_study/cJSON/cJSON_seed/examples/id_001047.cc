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
//<ID> 1047
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    const char *label_text = "sensor-A";
    cJSON *label_ref = cJSON_CreateStringReference(label_text);
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 2: Configure (parse external JSON inputs)
    const char *json1 = "{\"a\":5,\"b\":7}";
    cJSON *parsed1 = cJSON_Parse(json1);
    const char *buf2 = "123.5 extra";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithLengthOpts(buf2, (size_t)5, &parse_end, 0);

    // step 3: Operate & Validate (derive values and attach to root)
    cJSON *a_item = cJSON_GetObjectItem(parsed1, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    double b_val = cJSON_GetNumberValue(parsed2);
    double combined = a_val + b_val;
    cJSON *combined_num = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "combined", combined_num);
    int arr_count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "count", (double)arr_count);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}