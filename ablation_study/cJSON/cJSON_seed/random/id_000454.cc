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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s0 = NULL;
    cJSON *s1 = NULL;
    cJSON *ref_s1 = NULL;
    cJSON *status = NULL;
    char *printed = NULL;
    char *gotstr = NULL;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");

    // basic failure handling: ensure created objects exist
    if (!root || !arr || !s0 || !s1) {
        if (printed) cJSON_free((void *)printed);
        if (root) cJSON_Delete(root);
        else {
            if (arr) cJSON_Delete(arr);
            if (s0) cJSON_Delete(s0);
            if (s1) cJSON_Delete(s1);
        }
        return 0;
    }

    // step 3: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "items", arr);
    status = cJSON_AddStringToObject(root, "status", "ok");

    // Use a string reference for the existing string value instead of an object reference.
    // Creating an object reference for a string item can be invalid; use string reference API.
    ref_s1 = cJSON_CreateStringReference(cJSON_GetStringValue(s1));
    if (ref_s1) {
        cJSON_AddItemToArray(arr, ref_s1);
    }

    // step 4: Operate
    gotstr = cJSON_GetStringValue(status);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)gotstr;
    (void)printed;

    // step 6: Cleanup
    if (printed) {
        cJSON_free((void *)printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}