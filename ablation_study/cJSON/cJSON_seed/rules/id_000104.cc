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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON *name = cJSON_CreateString("test_object");
    cJSON *got_items = NULL;
    cJSON *got_name = NULL;
    cJSON *got_name_cs = NULL;
    char *buffer = NULL;
    int buf_len = 256;
    cJSON_bool is_name_string = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    got_items = cJSON_GetObjectItem(root, "items");
    cJSON_AddItemToArray(got_items, s3);
    got_name = cJSON_GetObjectItem(root, "name");
    got_name_cs = cJSON_GetObjectItemCaseSensitive(root, "name");
    is_name_string = cJSON_IsString(got_name_cs);
    buffer = (char*)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}