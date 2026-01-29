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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "list", list);

    // step 2: Configure
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(data, "double_array", dbl_arr);
    cJSON *count_item = cJSON_AddNumberToObject(data, "count", 3.0);
    cJSON *nitem = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(list, nitem);
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  { \"min\": null }  ");
    cJSON_Minify(json_buf);
    cJSON *raw = cJSON_CreateRaw(json_buf);
    cJSON_AddItemToArray(list, raw);

    // step 3: Operate & Validate
    cJSON *retrieved_count = cJSON_GetObjectItem(data, "count");
    double count_val = cJSON_GetNumberValue(retrieved_count);
    cJSON_AddNumberToObject(data, "count_plus_one", count_val + 1.0);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}