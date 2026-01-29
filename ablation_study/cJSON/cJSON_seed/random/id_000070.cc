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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "Alice", "Bob", "Carol" };
    cJSON *arr = cJSON_CreateStringArray(names, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = (cJSON *)0;
    cJSON *names_copy = (cJSON *)0;
    cJSON_bool ref_added = 0;
    double index_val = 0.0;

    // step 2: Initialize - add a numeric item to root
    num_item = cJSON_AddNumberToObject(root, "index", 7.0);

    // step 3: Configure - attach the string array and a boolean to root
    cJSON_AddItemToObject(root, "names", arr);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Reference - add a reference to the existing array under a different name
    ref_added = cJSON_AddItemReferenceToObject(root, "names_ref", arr);

    // step 5: Operate/Validate - read back the number and add a derived number; also create and attach a copy of the string array
    index_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "index_double", index_val * 2.0);
    names_copy = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names_copy", names_copy);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}