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
//<ID> 1123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *names_item = NULL;
    cJSON *status_item = NULL;
    cJSON *new_names = NULL;
    cJSON *false_item = NULL;
    char *printbuf = NULL;
    const char json[] = "{\"names\":[\"alpha\",\"beta\"],\"status\":true}";
    const char *parse_end = NULL;
    const char *new_strs_arr[] = { "one", "two", "three" };

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    names_item = cJSON_GetObjectItem(root, "names");
    status_item = cJSON_GetObjectItem(root, "status");
    new_names = cJSON_CreateStringArray(new_strs_arr, 3);
    false_item = cJSON_CreateFalse();

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, names_item, new_names);
    cJSON_ReplaceItemViaPointer(root, status_item, false_item);
    printbuf = (char *)cJSON_malloc(256);
    memset(printbuf, 0, 256);
    cJSON_PrintPreallocated(root, printbuf, 256, 0);
    (void)printbuf;

    // step 4: Cleanup
    cJSON_free(printbuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}