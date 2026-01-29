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
//<ID> 671
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON_bool equal = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *person2 = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30);
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddStringToObject(person2, "name", "Alice");
    cJSON_AddNumberToObject(person2, "age", 30);
    cJSON_AddItemToObject(root, "person_copy", person2);

    // step 3: Operate / Validate
    equal = cJSON_Compare(person, person2, 1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}