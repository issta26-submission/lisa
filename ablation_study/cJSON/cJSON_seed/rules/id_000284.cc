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
//<ID> 284
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
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *got_list = NULL;
    cJSON *version_item = NULL;
    double version_value = 0.0;
    int arr_size = 0;
    int version_int = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddStringToObject(root, "title", "cJSON example");
    cJSON_AddNumberToObject(root, "version", 7.0);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate / Validate
    cJSON_PrintPreallocated(root, buffer, 512, 0);
    got_list = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(got_list);
    version_item = cJSON_GetObjectItem(root, "version");
    version_value = cJSON_GetNumberValue(version_item);
    version_int = (int)version_value;
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('0' + (version_int % 10));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}