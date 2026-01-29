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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *custom_bool = cJSON_CreateBool(0);
    cJSON *detached = (cJSON *)0;
    int remaining = 0;
    char outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Setup - populate array
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, custom_bool);
    cJSON_AddItemToArray(arr, s3);

    // step 3: Configure - attach array to root and add a true item
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON_AddTrueToObject(root, "always_true");

    // step 4: Core operations - detach an item from the array and reattach under root
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_SetValuestring(detached, "detached_two");
    cJSON_AddItemToObjectCS(root, "detached_item", detached);

    // step 5: Validate - record remaining array size and print into preallocated buffer
    remaining = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "remaining", (double)remaining);
    cJSON_PrintPreallocated(root, outbuf, (int)sizeof(outbuf), 1);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}