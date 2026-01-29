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
//<ID> 1568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str0 = NULL;
    cJSON *trueItem = NULL;
    cJSON *retrieved = NULL;
    cJSON *copied = NULL;
    cJSON *marker_bool = NULL;
    char *buf = NULL;
    char *out = NULL;
    const char *sval = NULL;
    size_t len = 0;
    cJSON_bool isstr = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str0 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str0);
    trueItem = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", trueItem);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    sval = cJSON_GetStringValue(retrieved);
    isstr = cJSON_IsString(retrieved);
    len = (sval != NULL) ? (strlen(sval) + 1) : 1;
    buf = (char *)cJSON_malloc(len);
    memset(buf, 0, len);
    memcpy(buf, sval ? sval : "", len);
    copied = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "copied_first", copied);
    marker_bool = cJSON_CreateBool(isstr);
    cJSON_AddItemToObject(root, "first_is_string", marker_bool);

    // step 4: Cleanup
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}