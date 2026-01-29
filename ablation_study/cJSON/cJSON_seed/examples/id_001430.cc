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
//<ID> 1430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *template_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(template_obj, "v", 7.0);
    cJSON_AddItemToObject(root, "template", template_obj);
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 2: Configure
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);

    // step 3: Operate & Validate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    cJSON *dup_pi = cJSON_Duplicate(pi_item, 1);
    cJSON_bool is_num = cJSON_IsNumber(dup_pi);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON *added = cJSON_AddNumberToObject(root, "added", 99.0);
    (void)is_num;
    (void)added;

    // step 4: Cleanup
    cJSON_Delete(dup_pi);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}