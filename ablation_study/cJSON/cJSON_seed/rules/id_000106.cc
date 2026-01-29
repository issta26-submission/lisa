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
//<ID> 106
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("testname");
    char *buffer = (char *)cJSON_malloc(256);
    const int buf_len = 256;
    cJSON_bool is_name_string = 0;
    cJSON *got_meta = NULL;
    cJSON *got_name = NULL;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    got_name = cJSON_GetObjectItem(got_meta, "name");
    is_name_string = cJSON_IsString(got_name);
    cJSON_AddBoolToObject(root, "name_is_string", is_name_string);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}