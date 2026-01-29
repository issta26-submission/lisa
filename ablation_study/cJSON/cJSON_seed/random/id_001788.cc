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
//<ID> 1788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *mirror = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *mirror_str_item = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    cJSON *detached_str = (cJSON *)0;
    char *strval = (char *)0;
    double num_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    mirror = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("the answer");

    // step 3: Configure (root owns num and str; mirror holds references)
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "text", str);
    add_ok1 = cJSON_AddItemReferenceToObject(mirror, "numref", num);
    add_ok2 = cJSON_AddItemReferenceToObject(mirror, "strref", str);

    // step 4: Operate (retrieve string via mirror, detach references, read number)
    mirror_str_item = cJSON_GetObjectItem(mirror, "strref");
    strval = cJSON_GetStringValue(mirror_str_item);
    detached_num = cJSON_DetachItemFromObjectCaseSensitive(mirror, "numref");
    detached_str = cJSON_DetachItemFromObjectCaseSensitive(mirror, "strref");
    num_value = cJSON_GetNumberValue(detached_num);

    // step 5: Validate
    validation = (int)(strval != (char *)0) + (int)(detached_num != (cJSON *)0) + (int)(detached_str != (cJSON *)0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(num_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(mirror);
    cJSON_Delete(root);
    (void)root;
    (void)mirror;
    (void)num;
    (void)str;
    (void)mirror_str_item;
    (void)detached_num;
    (void)detached_str;
    (void)strval;
    (void)num_value;
    (void)add_ok1;
    (void)add_ok2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}