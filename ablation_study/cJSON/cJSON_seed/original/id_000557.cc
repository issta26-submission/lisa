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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"x\" : 10 , \"y\" : 20 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *val = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "value", val);
    double new_val = cJSON_SetNumberHelper(val, 123.456);
    cJSON_AddTrueToObject(root, "flag");
    double sum_components = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "x")) + cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "y")) + new_val;
    cJSON *sum_item = cJSON_CreateNumber(sum_components);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *parsed_snapshot = cJSON_Print(parsed);

    // step 4: Validate & Cleanup
    double extracted_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    (void)extracted_sum;
    cJSON_free(snapshot);
    cJSON_free(parsed_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}