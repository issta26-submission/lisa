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
//<ID> 558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = " { \"name\" : \"parsed_name\", \"value\" : 123 } ";
    cJSON_Minify(json_text);
    cJSON *parsed = cJSON_Parse(json_text);
    const char *parsed_name = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "name"));

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *name_ref = cJSON_CreateStringReference(parsed_name);
    cJSON_AddItemToObject(root, "name", name_ref);
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "value", num_item);
    double set_ret = cJSON_SetNumberHelper(num_item, 99.25);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    snprintf((char *)buffer, buf_len, "%s", snapshot);
    double valread = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double sum = set_ret + valread;
    cJSON_SetNumberHelper(num_item, sum);

    // step 4: Validate & Cleanup
    double final_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)final_val;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}