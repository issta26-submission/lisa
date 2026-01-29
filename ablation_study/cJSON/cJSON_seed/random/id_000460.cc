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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *next_item = (cJSON *)0;
    char *printed = (char *)0;
    double num_value = 0.0;
    cJSON_bool has_note = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example text");

    // step 3: Configure
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "note", str_item);

    // step 4: Operate
    num_value = cJSON_GetNumberValue((const cJSON *const)num_item);
    next_item = cJSON_CreateNumber(num_value + 1.0);
    cJSON_AddItemToObject(root, "next", next_item);
    has_note = cJSON_HasObjectItem(root, "note");

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_Minify(printed);
    (void)num_value;
    (void)has_note;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}