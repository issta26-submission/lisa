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
//<ID> 1433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "original", 10.5);
    cJSON *orig = cJSON_GetObjectItem(root, "original");

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(orig, 0);
    cJSON_AddItemToObject(root, "duplicate", dup);

    // step 3: Operate & Validate
    cJSON_bool isnum1 = cJSON_IsNumber(orig);
    cJSON_bool isnum2 = cJSON_IsNumber(dup);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_AddStringToObject(root, "snapshot", buffer);
    cJSON_SetNumberHelper(orig, 20.0);
    cJSON_bool isnum3 = cJSON_IsNumber(orig);
    (void)isnum1;
    (void)isnum2;
    (void)printed;
    (void)isnum3;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}