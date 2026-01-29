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
//<ID> 634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *printed_first = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *s = cJSON_CreateString("example");
    cJSON *first = NULL;
    size_t root_len = 0;
    size_t first_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_first = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(data, "name", s);
    cJSON_AddItemToObject(data, "values", arr);
    cJSON_AddItemToObject(root, "data", data);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    printed_first = cJSON_PrintUnformatted(first);
    printed_root = cJSON_PrintUnformatted(root);
    root_len = printed_root ? strlen(printed_root) : 0;
    first_len = printed_first ? strlen(printed_first) : 0;
    to_copy_root = root_len < 255 ? root_len : 255;
    to_copy_first = (first_len < (511 - to_copy_root)) ? first_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, printed_first ? printed_first : "", to_copy_first);
    scratch[to_copy_root + to_copy_first] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_first);
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}