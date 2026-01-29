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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    /* Build a JSON object:
       {
         "name": "Alice",
         "age": 30,
         "married": false,
         "children": ["Bob", "Carol"],
         "address": { "city": "Wonderland", "zip": 12345 }
       }
    */

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    /* Simple items */
    cJSON_AddStringToObject(root, "name", "Alice");
    cJSON_AddNumberToObject(root, "age", 30);

    /* Boolean item (create then add) */
    cJSON *married = cJSON_CreateBool(0); /* false */
    if (!married) {
        cJSON_Delete(root);
        return 2;
    }
    cJSON_AddItemToObject(root, "married", married);

    /* Array of children */
    cJSON *children = cJSON_CreateArray();
    if (!children) {
        cJSON_Delete(root);
        return 3;
    }
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Carol"));
    cJSON_AddItemToObject(root, "children", children);

    /* Nested object: address */
    cJSON *address = cJSON_CreateObject();
    if (!address) {
        cJSON_Delete(root);
        return 4;
    }
    cJSON_AddStringToObject(address, "city", "Wonderland");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    /* Serialize to string */
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) {
        cJSON_Delete(root);
        return 5;
    }

    /* Parse back the printed JSON */
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        free(printed);
        cJSON_Delete(root);
        return 6;
    }

    /* Validate parsed contents */

    /* name */
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || (std::strcmp(name->valuestring, "Alice") != 0)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 7;
    }

    /* age */
    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age) || ((int)age->valuedouble != 30)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 8;
    }

    /* children array */
    cJSON *children_p = cJSON_GetObjectItemCaseSensitive(parsed, "children");
    if (!children_p || cJSON_GetArraySize(children_p) != 2) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 9;
    }
    cJSON *child0 = cJSON_GetArrayItem(children_p, 0);
    cJSON *child1 = cJSON_GetArrayItem(children_p, 1);
    if (!cJSON_IsString(child0) || !cJSON_IsString(child1) ||
        std::strcmp(child0->valuestring, "Bob") != 0 ||
        std::strcmp(child1->valuestring, "Carol") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 10;
    }

    /* address nested object */
    cJSON *address_p = cJSON_GetObjectItemCaseSensitive(parsed, "address");
    if (!address_p) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 11;
    }
    cJSON *city = cJSON_GetObjectItemCaseSensitive(address_p, "city");
    cJSON *zip = cJSON_GetObjectItemCaseSensitive(address_p, "zip");
    if (!cJSON_IsString(city) || (std::strcmp(city->valuestring, "Wonderland") != 0) ||
        !cJSON_IsNumber(zip) || ((int)zip->valuedouble != 12345)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 12;
    }

    /* Cleanup */
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    /* If we reached here, the sequence of cJSON API calls succeeded and validation passed */
    return 66;
}