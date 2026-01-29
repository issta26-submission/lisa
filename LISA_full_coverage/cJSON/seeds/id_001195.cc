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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = " { \"arr\" : [ \"one\" , \"two\" , \"three\" ] } ";
    cJSON_Minify(json_text);
    cJSON *root = cJSON_Parse(json_text);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_AddItemToArray(arr, inserted);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}