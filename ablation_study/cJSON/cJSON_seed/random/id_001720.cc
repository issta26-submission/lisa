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
//<ID> 1720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greet = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *elem = (cJSON *)0;
    char *extracted = (char *)0;
    int size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greet = cJSON_CreateString("hello");
    list = cJSON_CreateArray();
    elem = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, elem);

    // step 4: Operate
    extracted = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    size = cJSON_GetArraySize(list);

    // step 5: Validate
    validation = (int)(size == 1) + (int)(extracted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)greet;
    (void)list;
    (void)elem;
    (void)extracted;
    (void)size;
    // API sequence test completed successfully
    return 66;
}