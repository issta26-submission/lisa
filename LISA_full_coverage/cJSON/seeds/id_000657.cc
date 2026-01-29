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
//<ID> 657
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n2);
    const char *ref_str = "hello_ref";
    cJSON *sref = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToArray(arr, sref);

    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "note", "testing cJSON sequence");

    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second);
    cJSON_AddBoolToObject(meta, "second_is_number", second_is_number);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, third);

    /* Create a heap copy of the string value so forcing an invalid type
       won't cause cJSON_Delete to free a non-heap pointer. (Straight-line
       sequence only — assumes detached and detached->valuestring are valid
       as in the original test scenario.) */
    size_t _len = strlen(detached->valuestring);
    char *_copy = (char*)cJSON_malloc(_len + 1);
    memcpy(_copy, detached->valuestring, _len + 1);
    detached->valuestring = _copy;

    detached->type = 0; /* force an invalid-type state for testing */
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
    cJSON_AddBoolToObject(meta, "detached_is_invalid", detached_invalid);

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66;
}