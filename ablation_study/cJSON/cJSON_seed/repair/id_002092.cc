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
//<ID> 2092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *inserted_str = cJSON_CreateString("inserted_at_one");
    cJSON_AddItemToArray(array, cJSON_CreateString("first"));
    cJSON_AddItemToArray(array, cJSON_CreateString("third"));
    cJSON_InsertItemInArray(array, 1, inserted_str);
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "cJSON_Preallocated_Test");
    cJSON *note = cJSON_CreateString("note_string");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    char first_char = buffer[0];
    (void)first_char;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}